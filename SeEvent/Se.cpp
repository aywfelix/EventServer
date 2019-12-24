#include "Se.h"
#include "Socket.h"
#include "LogHelper.h"
#include "Session.h"
#include "LogHelper.h"
#include "SocketDefine.h"
#include "Session.h"
#include "MemPool.hpp"

bool SeEventOp::Init()
{
	mMaxFd = INVALID_SOCKET;
	// init timeval
	gettimeofday(&mtv, NULL);
	mtv.tv_sec = TIMEOUT;
	mtv.tv_usec = 0;
	this->InitOp();
	return true;
}

void SeEventOp::SetMaxFd(socket_t fd)
{
	if (fd != INVALID_SOCKET)
	{
		if (fd > mMaxFd)
		{
			mMaxFd = fd;
		}
	}
}

void SeEventOp::SetActiveEvent(socket_t fd, int mask)
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

std::map<socket_t, int>& SeEventOp::GetActiveEvents()
{
	return mActiveEvents;
}

bool SeEventOp::Dispatch()
{

	return this->Dispatch(&mtv);
}

void seEventLoop::Init(SeEventOp* pEventOp)
{
	mbStop = false;
	if (pEventOp)
	{
		mEventOp = pEventOp;
	}
#if SF_PLATFORM == SF_PLATFORM_WIN
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	mSocket = new Socket;
	mSocket->CreateFd();
	mEventOp->SetMaxFd(mSocket->GetFd());
#ifdef _WIN32
	mEventOp->AddEvent(mSocket->GetFd(), EV_READ | EV_WRITE);
#else
	AddEvent(mSocket->GetFd(), EPOLLET | EPOLLONESHOT | EPOLLIN);
#endif
}

bool seEventLoop::InitServer(UINT port)
{
	mbServer = true;
	mSocket->SetNonBlock();
	if (!mSocket->Listen(port))
	{
		return false;
	}
	mSocket->SetReUseAddr();
	LOG_INFO("init server ok ....");
	return true;
}

bool seEventLoop::InitClient(const char* ip, UINT port)
{
	if (!mSocket->Connect(ip, port))
	{
		return false;
	}
	mSocket->SetNonBlock();
	AddSession(mSocket);
	LOG_INFO("init client ....");
	return true;
}

void seEventLoop::AddSession(Socket* pSocket)
{
	Session* pSession = g_pSessionPool->NewSession();
	pSession->SetSocket(pSocket);
	mSessions.emplace(pSocket->GetFd(), pSession);
}

bool seEventLoop::AcceptClient()
{
	socket_t connfd = INVALID_SOCKET;
	struct sockaddr_in addr;
	if (mSocket->Accept(connfd, addr))
	{
		Socket* pSocket = new Socket;
		pSocket->SetSocket(connfd, addr);
		pSocket->SetNonBlock();
		AddSession(pSocket);
		mEventOp->SetMaxFd(connfd);
		LOG_INFO("accept client connect ...%d", connfd);
		return true;
	}
	return false;
}


void seEventLoop::StartLoop()
{
	while (!mbStop)
	{
		mEventOp->Dispatch();
		auto activemq = mEventOp->GetActiveEvents();
		// do with activemq
		for (auto it = activemq.begin(); it != activemq.end();)
		{
			if (it->second & EV_READ)
			{
				if (mSocket->GetFd() == it->first && mbServer)
				{
					AcceptClient();
				}
				// if socket cache is readable then read data
				int nRecvSize = GetReadableSizeOnSocket(it->first);
				if (nRecvSize > 0)
				{
					LOG_INFO("can read data len==%d", nRecvSize);

				}
				LOG_INFO("read event trigger....%d", it->first);
			}
			if (it->second & EV_WRITE)
			{
				LOG_INFO("write event trigger....%d", it->first);
				for (auto it : mSessions)
				{
					send(it.first, "1234567890", 10, 0);
				}
				mEventOp->DelEvent(it->first, EV_WRITE);
			}
			if (it->second & EV_CLOSED)
			{
				LOG_INFO("close event trigger....%d", it->first);
			}
			it = activemq.erase(it);
		}
	}
}


void seEventLoop::StopLoop()
{
	mEventOp->Clear();
	mbStop = true;
}

