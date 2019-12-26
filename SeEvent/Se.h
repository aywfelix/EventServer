#pragma once

#include <ctime>
#include <map>
#include <memory>
#include <functional>

#include "SePlatForm.h"

#define SE_OK 0
#define SE_ERR -1

#define EV_NONE 0
#define EV_TIMEOUT 1
#define EV_READ	2
#define EV_WRITE 4
#define EV_CLOSED 8

#define TIMEOUT 5

typedef int64_t SOCK;

enum SE_NET_EVENT
{
	SE_NET_EVENT_EOF = 0x10,	/**< eof file reached */
	SE_NET_EVENT_ERROR = 0x20,	/**< unrecoverable error encountered */
	SE_NET_EVENT_TIMEOUT = 0x40,	/**< user-specified timeout reached */
	SE_NET_EVENT_CONNECTED = 0x80,	/**< connect operation finished. */
};

class seEventLoop;
class Socket;
class Session;

using NET_RECEIVE_FUNCTOR = std::function<void(const SOCK nFd, const int nMsgId, const char* pMsg, const uint32_t nLen)>;
using NET_RECEIVE_FUNCTOR_PTR = std::shared_ptr<NET_RECEIVE_FUNCTOR>;

using NET_EVENT_FUNCTOR = std::function<void(const SOCK nFd, const SE_NET_EVENT nEvent, seEventLoop* pNet)>;
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

class seEventLoop
{
public:
	void Init();
	bool InitServer(UINT port);
	bool InitClient(const char* ip, UINT port);
	void StartLoop();
	void StopLoop();
	
private:
	void AddSession(Socket* pSocket);
	Session* GetSession(socket_t fd);
	void CloseSession(Session* pSession);
	void AcceptClient();

	void EventRead(Session* pSession);
	void EventWrite(Session* pSession);

private:
	bool mbStop;
	SeEventOp* mEventOp;
	Socket* mSocket;
	std::map<socket_t, Session*> mSessions;
	bool mbServer{ false };

	NET_RECEIVE_FUNCTOR mRecvCB;
	NET_EVENT_FUNCTOR mEventCB;
};
