#include "Se.h"
#include "Socket.h"
#include "LogHelper.h"
#include "Session.h"
#include "LogHelper.h"

bool SeEventOp::Init(seEventLoop* pEventLoop, int timeout)
{
	mMaxFd = INVALID_SOCKET;
	mEventLoop = pEventLoop;
#if SF_PLATFORM == SF_PLATFORM_WIN
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	mSocket = new Socket;
	mSocket->CreateFd();
	// init timeval
	gettimeofday(&mtv, NULL);
	mtv.tv_sec = timeout;
	mtv.tv_usec = 0;
	this->InitOp();
	return true;
}

bool SeEventOp::InitServer(UINT port)
{
	mbServer = true;
	mSocket->SetNonBlock();
	if (!mSocket->Listen(port))
	{
		return false;
	}
	mSocket->SetReUseAddr();
	mMaxFd = mSocket->GetFd();
#ifndef _WIN32

#endif
	InitOp();
	AddEvent(mSocket->GetFd(), EV_READ | EV_WRITE);
	LOG_INFO("init server %d", mMaxFd);
	return true;
}

bool SeEventOp::InitClient(const char* ip, UINT port)
{
	if (!mSocket->Connect(ip, port))
	{
		return false;
	}
	mSocket->SetNonBlock();
	mMaxFd = mSocket->GetFd();
	InitOp();
	AddEvent(mSocket->GetFd(), EV_READ | EV_WRITE);
	LOG_INFO("init client %d", mMaxFd);
	return true;
}

bool SeEventOp::AcceptClient(socket_t& connfd)
{
	struct sockaddr_in addr;
	if (mSocket->Accept(connfd, addr))
	{
		Socket* pSocket = new Socket;
		pSocket->SetSocket(connfd, addr);
		pSocket->SetNonBlock();
		Session* pSession = g_pSessionPool->GetSession();
		pSession->SetSocket(pSocket);
		mSessions.emplace(connfd, pSession);
		return true;
	}
	return false;
}

bool SeEventOp::Dispatch()
{
	return this->Dispatch(&mtv);
}

void seEventLoop::Init()
{
	mbStop = false;
}

void seEventLoop::StartLoop()
{
	while (!mbStop)
	{
		mEventOp->Dispatch();
		// do with activemq
		for (auto it = mActiveEvents.begin(); it != mActiveEvents.end();)
		{
			if (it->second & EV_READ)
			{
				LOG_INFO("read event trigger....%d", it->first);
			}
			if (it->second & EV_WRITE)
			{
				LOG_INFO("write event trigger....%d", it->first);
				mEventOp->DelEvent(it->first, EV_WRITE);
			}
			if (it->second & EV_CLOSED)
			{
				LOG_INFO("close event trigger....%d", it->first);
			}
			it = mActiveEvents.erase(it);
		}
	}
}

void seEventLoop::SetEventOp(SeEventOp* pEventOp)
{
	if (pEventOp)
	{
		mEventOp = pEventOp;
	}
}

void seEventLoop::StopLoop()
{
	mEventOp->Clear();
	mbStop = true;
}

void seEventLoop::SetActiveEvent(socket_t fd, int mask)
{
	auto it = mActiveEvents.find(fd);
	if (it == mActiveEvents.end())
	{
		mActiveEvents[fd] = mask;
	}
	else
	{
		it->second = mask;
	}
}

