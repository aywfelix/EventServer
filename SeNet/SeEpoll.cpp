#include "SeNet.h"
#include "SeEpoll.h"
#include "SePlatForm.h"
#include "Session.h"
#include "Assertx.h"

#ifdef SF_PLATFORM_LINUX

SeEpoll::SeEpoll()
{
}
SeEpoll::~SeEpoll()
{
}

bool SeEpoll::InitOp()
{
#ifdef EVENT_HAVE_EPOLL_CREATE1
	mEpollOp.epfd = epoll_create1(EPOLL_CLOEXEC);
#endif
	if (mEpollOp.epfd == -1)
	{
		mEpollOp.epfd = epoll_create(1024);
		Assert(mEpollOp.epfd != -1);
	}
	mEpollOp.events = new epoll_event[EPOLL_EVENT_NUM];
	if (mEpollOp.events == nullptr)
	{
		return false;
	}
	return true;
}

bool SeEpoll::AddEvent(socket_t fd, int mask£¬ void* ptr)
{
	uint32_t events = 0;
	if (mask & EV_READ)
	{
		events = EPOLLET | EPOLLONESHOT | EPOLLIN;
	}
	if (mask & EV_WRITE)
	{
		events = EPOLLET | EPOLLONESHOT | EPOLLOUT;
	}
	struct epoll_event ev = { 0 };
	short op = 0;
	auto it = mEvents.find(fd);
	if (it == mEvents.end())
	{
		ev.data.fd = fd;
		ev.events = events; // EPOLLET | EPOLLONESHOT | EPOLLIN | EPOLLOUT
		mEvents.emplace(fd, ev);
		op = EPOLL_CTL_ADD;
	}
	else
	{
		ev = it->second;
		ev.events = events;
		op = EPOLL_CTL_MOD;
	}
	if (epoll_ctl(mEpollOp.epfd, op, fd, &ev) == 0)
	{
		return true;
	}
	switch (op) {
	case EPOLL_CTL_MOD:
		if (errno == ENOENT) {
			if (epoll_ctl(mEpollOp.epfd, EPOLL_CTL_ADD, fd, &ev) == -1) {
				return false;
			}
			return true;
		}
		break;
	case EPOLL_CTL_ADD:
		if (errno == EEXIST) {
			if (epoll_ctl(mEpollOp.epfd, EPOLL_CTL_MOD, fd, &ev) == -1) {
				return false;
			}
			return true;
		}
		break;
	case EPOLL_CTL_DEL:
		if (errno == ENOENT || errno == EBADF || errno == EPERM) {
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool SeEpoll::DelEvent(socket_t fd, int mask)
{
	auto it = mEvents.find(fd);
	if (it == mEvents.end())
	{
		return false;
	}
	if (epoll_ctl(mEpollOp.epfd, EPOLL_CTL_DEL, fd, nullptr) == 0)
	{
		return true;
	}
	return false;
}

bool SeEpoll::Dispatch(struct timeval* tv)
{
	int ret, nEvents = 0;
	ret = epoll_wait(mEpollOp.epfd, mEpollOp.events, EPOLL_EVENT_NUM,
		tv ? (tv->tv_sec * 1000 + tv->tv_usec / 1000) : -1);
	if (ret == -1)
	{
		if (SOCKET_ERR_RW_RETRIABLE(errno))
		{
			return true;
		}
		fprintf(stderr, "epoll_wait error %d:%s", errno, strerror(errno));
		return false;
	}
	if (ret > 0)
	{
		nEvents = ret;
		for (int i = 0; i < nEvents; i++)
		{
			struct epoll_event ev = mEpollOp.events[i];
			int mask = 0;
			if (ev.events & (EPOLLHUP | EPOLLERR))
			{
				mask = EV_READ | EV_WRITE;
			}
			else
			{
				if (ev.events & EPOLLIN)
				{
					mask |= EV_READ;
					AddEvent(ev.data.fd, mask);
				}
				if (ev.events & EPOLLOUT)
				{
					mask |= EV_WRITE;
					AddEvent(ev.data.fd, mask);
				}
				if (ev.events & EPOLLRDHUP)
				{
					mask |= EV_CLOSED;
					DelEvent(ev.data.fd, EPOLLRDHUP);
				}
			}
			if (mask == 0)
			{
				continue;
			}
			SetEvent(ev.data.fd, mask);
		}
	}
	return true;
}

bool SeEpoll::Clear()
{
	SocketCloseOnExec(mEpollOp.epfd);
	mEpollOp.epfd = -1;
	delete[] mEpollOp.events;
	return true;
}

#endif