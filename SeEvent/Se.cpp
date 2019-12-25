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
	mEventOp->AddEvent(mSocket->GetFd(), EV_READ);
#else
	mEventOp->AddEvent(mSocket->GetFd(), EV_READ);
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
	if (pSession == nullptr)
		return;
	pSession->SetSocket(pSocket);
	mSessions.emplace(pSocket->GetFd(), pSession);
}

Session* seEventLoop::GetSession(socket_t fd)
{
	auto it = mSessions.find(fd);
	if (it == mSessions.end())
	{
		return nullptr;
	}
	return it->second;
}

void seEventLoop::CloseSession(Session* pSession)
{
	g_pSessionPool->DelSession(pSession);
	mEventOp->DelEvent(pSession->GetSocket()->GetFd(), EV_READ | EV_WRITE);
}

void seEventLoop::AcceptClient()
{
	for (;;)
	{
		socket_t connfd = INVALID_SOCKET;
		struct sockaddr_in addr;
		if (mSocket->Accept(connfd, addr))
		{
			Socket* pSocket = new Socket;
			Assert(pSocket != nullptr);
			pSocket->SetSocket(connfd, addr);
			pSocket->SetNonBlock();
			AddSession(pSocket);
			mEventOp->SetMaxFd(connfd);
#ifdef _WIN32
			mEventOp->AddEvent(mSocket->GetFd(), EV_READ);
#else
			mEventOp->AddEvent(mSocket->GetFd(), EV_READ);
#endif
			LOG_INFO("accept client connect ...%d", connfd);
			continue;
		}
		break;
	}
}


void seEventLoop::StartLoop()
{
	while (!mbStop)
	{
		if (!mEventOp->Dispatch())
		{
			break;
		}
		auto activemq = mEventOp->GetActiveEvents();
		// do with activemq
		for (auto it = activemq.begin(); it != activemq.end();)
		{
			Session* pSession = GetSession(it->first);
			if (pSession == nullptr)
			{
				LOG_WARN("eventloop session is null, socket==%d", it->first);
				continue;
			}

			if (it->second & EV_READ)
			{
				LOG_INFO("read event trigger....%d", it->first);
				if (mSocket->GetFd() == it->first && mbServer)
				{
					AcceptClient();
				}
				// if socket cache is readable then read data

				
				int nRecvSize = GetReadableSizeOnSocket(it->first);
				int nRecvLeft = 0;
				if (nRecvSize > 0)
				{
					LOG_INFO("can read data len==%d", nRecvSize);
					for (;;)
					{
						char* buf = pSession->GetRecvBuf(nRecvLeft);
						int ret = recv(it->first, buf, nRecvLeft, 0);
						if (ret < 0)
						{
							if (errno == EINTR)
							{
								continue;
							}
							if (errno == EAGAIN || errno == EWOULDBLOCK)
							{
								LOG_INFO("select recv data finish!");
								pSession->PostRecvData(nRecvSize);
								break;
							}
							LOG_WARN("select recv error!");
							CloseSession(pSession);
						}
						else if (ret == 0)
						{
							LOG_WARN("connection peer closed!");
							CloseSession(pSession);
							break;
						}
						nRecvLeft -= ret;
						LOG_INFO("select recv %d byte of content %s", ret, buf);
					}
				}
			}
			if (it->second & EV_WRITE)
			{
				LOG_INFO("write event trigger....%d", it->first);
				int nSendSize = 0;
				char* pSendBuf = pSession->GetSendBuf(nSendSize);
				while (pSession && pSendBuf)
				{
					int ret = send(it->first, pSendBuf, nSendSize, 0);
					if (ret <= 0)
					{
						break;
					}
					pSession->PostSendData(ret);
					nSendSize = 0;
					pSendBuf = pSession->GetSendBuf(nSendSize);
					LOG_INFO("select write to socket....%d len, contents %s", pSendBuf);
				}
			}
			if (it->second & EV_CLOSED)
			{
				LOG_INFO("close event trigger....%d", it->first);
				CloseSession(pSession);
			}
			it = activemq.erase(it);
		}
	}
}

void seEventLoop::StopLoop()
{
	mEventOp->Clear();
	mbStop = true;
	mSocket->CloseSocket();
	for (auto it : mSessions)
	{
		g_pSessionPool->DelSession(it.second);
	}
}

