#pragma once

#include <ctime>
#include <map>
#include <vector>
#include <memory>
#include <functional>
#include <string>

#include "SocketDefine.h"
#include "google/protobuf/message.h"
#include "SeFINet.h"

class Socket;
class Session;

class SeEventOp
{
public:
	SeEventOp();
	virtual ~SeEventOp();
	virtual bool Init();
	bool Dispatch();
	virtual bool InitOp() = 0;
	virtual bool AddEvent(socket_t fd, int mask) = 0;
	virtual bool DelEvent(socket_t fd, int mask) = 0;
	virtual bool Dispatch(struct timeval* tv) = 0;
	virtual bool Clear() = 0;
	void SetEvent(socket_t fd, int mask);
	std::map<socket_t, int>& GetEvents();
	void SetMaxFd(socket_t fd);
protected:
	socket_t m_maxfd;
	struct timeval mtv;
	std::map<socket_t, int> m_events;
};

class SeNet
{
public:

	SeNet(NET_RECEIVE_FUNCTOR& receiveCb, NET_EVENT_FUNCTOR& eventCb)
	{
		mRecvCB = receiveCb;
		mEventCB = eventCb;
	}
	template<typename BaseType>
	SeNet(BaseType* pBaseType, void (BaseType::* HandleReceive)(const socket_t, const int, const char*, const size_t), void (BaseType::* HandleEvent)(const socket_t, const SE_NET_EVENT, SeNet*))
	{
		mRecvCB = std::bind(HandleReceive, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		mEventCB = std::bind(HandleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
	bool InitServer(uint32_t port);
	bool InitClient(const char* ip, uint32_t port);
	void StartLoop(LOOP_RUN_TYPE run);
	void StopLoop();

	// send msg with proto head
	void SendProtoMsg(socket_t fd, const int msg_id, const char* msg, int len);
	void SendProtoMsg(std::vector<socket_t>& fdlist, const int msg_id, const char* msg, int len);
	void SendProtoMsg(const int msg_id, const char* msg, int len);

	// close or kick socket
	void CloseClient(socket_t fd);
	void CloseAllClient();

	Session* GetSession(socket_t fd);
	Socket* GetSocket() {
		return mSocket;
	}
private:
	Socket* InitSeNet();
	void AddSession(Socket* pSocket);
	
	void CloseSession(Session* pSession);
	void AcceptClient();

	void EventRead(Session* pSession);
	void EventWrite(Session* pSession);

	bool Dismantle(Session* pSession);
	// send msg
	void SendMsg(const char* msg, int len);
	void SendMsg(socket_t fd, const char* msg, int len);
	void SendMsg(std::vector<socket_t>& fds, const char* msg, int len);
	void SendToAllClients(const char* msg, int len);
	
private:
	bool mbStop{false};
	SeEventOp* mEventOp{nullptr};
	Socket* mSocket{nullptr};
	std::map<socket_t, Session*> m_sessions;  // 如果是服务器端，sessions保存所有连接，如果是客户端则只有一个session保存
	bool mbServer{ false };

	NET_RECEIVE_FUNCTOR mRecvCB;
	NET_EVENT_FUNCTOR mEventCB;
};
