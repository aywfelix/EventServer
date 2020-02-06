#pragma once

#ifdef SF_PLATFORM_LINUX
#include <sys/epoll.h>
#endif

#include <map>

#define EPOLL_EVENT_NUM 256

struct SeEpollOp
{
	int epfd{-1};
    struct epoll_event *events{nullptr};
};

class SeEpoll : public SeEventOp
{
public:
	SeEpoll();
	~SeEpoll();
    virtual bool InitOp();
    virtual bool AddEvent(socket_t fd, int mask);
    virtual bool DelEvent(socket_t fd, int mask);
    virtual bool Dispatch(struct timeval* tv);
    virtual bool Clear();
private:
    SeEpollOp mEpollOp;
	std::map<socket_t, struct epoll_event> mEvents;
};