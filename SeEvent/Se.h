#pragma once

#include <ctime>
#include <map>
#include "SePlatForm.h"


#define SE_OK 0
#define SE_ERR -1

#define EV_NONE 0
#define EV_TIMEOUT 1
#define EV_READ	2
#define EV_WRITE 4
#define EV_PERSIST 8
#define EV_ET 16
#define EV_FINALIZE 32
#define EV_CLOSED	64

#define SE_EVENT_READING	0x01	/**< error encountered while reading */
#define SE_EVENT_WRITING	0x02	/**< error encountered while writing */
#define SE_EVENT_EOF		0x10	/**< eof file reached */
#define SE_EVENT_ERROR		0x20	/**< unrecoverable error encountered */
#define SE_EVENT_TIMEOUT	0x40	/**< user-specified timeout reached */
#define SE_EVENT_CONNECTED	0x80	/**< connect operation finished. */


class seEventLoop;
class Socket;
class Session;

#define TIMEOUT 5

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
	void Init(SeEventOp* pEventOp);
	bool InitServer(UINT port);
	bool InitClient(const char* ip, UINT port);
	void StartLoop();
	void StopLoop();
private:
	void AddSession(Socket* pSocket);
	bool AcceptClient();
private:
	bool mbStop;
	SeEventOp* mEventOp;
	Socket* mSocket;
	std::map<socket_t, Session*> mSessions;
	bool mbServer{ false };
};
