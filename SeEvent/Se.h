#pragma once

#include <ctime>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "SePlatForm.h"
#include "google/protobuf/message.h"

#define SE_OK 0
#define SE_ERR -1

#define EV_NONE 0
#define EV_TIMEOUT 1
#define EV_READ	2
#define EV_WRITE 4
#define EV_CLOSED 8

#define TIMEOUT 5

enum LOOP_RUN_TYPE
{
	LOOP_RUN_BLOCK = 1,
	LOOP_RUN_NONBLOCK = 2,
};

enum SE_NET_EVENT
{
	SE_NET_EVENT_EOF = 0x10,	/**< eof file reached */
	SE_NET_EVENT_ERROR = 0x20,	/**< unrecoverable error encountered */
	SE_NET_EVENT_TIMEOUT = 0x40,	/**< user-specified timeout reached */
	SE_NET_EVENT_CONNECTED = 0x80,	/**< connect operation finished. */
};

class SeNet;
class Socket;
class Session;

using NET_RECEIVE_FUNCTOR = std::function<void(const socket_t nFd, const int nMsgId, const char* pMsg, const uint32_t nLen)>;
using NET_RECEIVE_FUNCTOR_PTR = std::shared_ptr<NET_RECEIVE_FUNCTOR>;

using NET_EVENT_FUNCTOR = std::function<void(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)>;
using NET_EVENT_FUNCTOR_PTR = std::shared_ptr<NET_EVENT_FUNCTOR>;


class SeEventOp
{
public:
	virtual bool Init();
	bool Dispatch();
	virtual bool InitOp() = 0;
	virtual bool AddEvent(socket_t fd, int mask) = 0;
	virtual bool DelEvent(socket_t fd, int mask) = 0;
	virtual bool Dispatch(struct timeval* tv) = 0;
	virtual bool Clear() = 0;
	void SetActiveEvent(socket_t fd, int mask);
	std::map<socket_t, int>& GetActiveEvents();
	void SetMaxFd(socket_t fd);
protected:
	socket_t mMaxFd;
	struct timeval mtv;
	std::map<socket_t, int> mActiveEvents;
};

class SeNet
{
public:
	SeNet(){}
	SeNet(NET_RECEIVE_FUNCTOR& receiveCb, NET_EVENT_FUNCTOR& eventCb)
	{
		mRecvCB = receiveCb;
		mEventCB = eventCb;
	}
	template<typename BaseType>
	SeNet(BaseType* pBaseType, void (BaseType::* HandleReceive)(const socket_t, const int, const char*, const UINT32), void (BaseType::* HandleEvent)(const socket_t, const SE_NET_EVENT, SeNet*))
	{
		mRecvCB = std::bind(HandleReceive, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		mEventCB = std::bind(HandleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
	bool InitServer(UINT port);
	bool InitClient(const char* ip, UINT port);
	void StartLoop(LOOP_RUN_TYPE run);
	void StopLoop();

	// send msg with proto head
	void SendProtoMsg(socket_t fd, const int nMsgID, const char* msg, int len);
	void SendProtoMsg(std::vector<socket_t>& fdlist, const int nMsgID, const char* msg, int len);
	void SendProtoMsg(const int nMsgID, const char* msg, int len);
	// recv pb msg
	static bool ReceivePB(const int nMsgID, const std::string& strMsg, google::protobuf::Message* pMsg);
	static bool ReceivePB(const int nMsgID, const char* msg, const UINT32 nLen, google::protobuf::Message* pData);
	// close or kick socket
	void CloseClient(socket_t fd);
	void CloseAllClient();

	Session* GetSession(socket_t fd);
private:
	void InitEventOp();
	void AddSession(Socket* pSocket);
	
	void CloseSession(Session* pSession);
	void AcceptClient();

	void EventRead(Session* pSession);
	void EventWrite(Session* pSession);

	bool Dismantle(Session* pSession);
	// send msg
	void SendMsg(const char* msg, int len);
	void SendMsg(socket_t fd, const char* msg, int len);
	void SendMsg(std::vector<socket_t>& fdlist, const char* msg, int len);
	void SendToAllClients(const char* msg, int len);

	
private:
	bool mbStop;
	SeEventOp* mEventOp;
	Socket* mSocket;
	std::map<socket_t, Session*> mSessions;  // 如果是服务器端，sessions保存所有连接，如果是客户端则只有一个session保存
	bool mbServer{ false };

	NET_RECEIVE_FUNCTOR mRecvCB;
	NET_EVENT_FUNCTOR mEventCB;
};
