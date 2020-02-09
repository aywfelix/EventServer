#include "SeNet.h"
#include "Socket.h"
#include "LogUtil.h"
#include "Session.h"
#include "LogUtil.h"
#include "SocketDefine.h"
#include "Session.h"
#include "MemPool.hpp"

#include "SeSelect.h"
#include "SeEpoll.h"
#include "SeFINet.h"

SeEventOp::SeEventOp() {}
SeEventOp::~SeEventOp()
{
	m_events.clear();
}

bool SeEventOp::Init()
{
	m_maxfd = INVALID_SOCKET;
	// init timeval
	gettimeofday(&mtv, nullptr);
	mtv.tv_sec = 0;
	mtv.tv_usec = LOOP_TIMEOUT*1000;
	this->InitOp();
	return true;
}

void SeEventOp::SetMaxFd(socket_t fd)
{
	if (fd != INVALID_SOCKET && fd > m_maxfd)
	{
		m_maxfd = fd;
	}
}

void SeEventOp::SetEvent(socket_t fd, int mask)
{
	m_events[fd] = mask;
}

std::map<socket_t, int>& SeEventOp::GetEvents()
{
	return m_events;
}

bool SeEventOp::Dispatch()
{
	return this->Dispatch(&mtv);
}

Socket* SeNet::InitSeNet()
{
#ifdef SF_PLATFORM_WIN
	mEventOp = new SeSelect();
#else
	mEventOp = new SeEpoll();
#endif
	Assert(mEventOp != nullptr);
	mEventOp->Init();
#if defined SF_PLATFORM_WIN
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data)!=0)
	{
		AssertEx(0, "windows init socket api error");
	}
#endif
	mbStop = false;
	Socket* pSocket = new Socket;
	Assert(pSocket != nullptr);
	pSocket->CreateFd();
	mEventOp->SetMaxFd(pSocket->GetFd());
#ifdef SF_PLATFORM_WIN
	mEventOp->AddEvent(pSocket->GetFd(), EV_READ);
#else
	mEventOp->AddEvent(pSocket->GetFd(), EV_READ);
#endif
	LOG_INFO("create socket %d", pSocket->GetFd());
	if (mbServer) mSocket = pSocket;
	return pSocket;
}

bool SeNet::InitServer(uint32_t port)
{
	mbServer = true;
	InitSeNet();
	if (!mSocket->Listen(port))
	{
		AssertEx(0, "Server listen error");
	}
	mSocket->SetSocketOptions();
	return true;
}

bool SeNet::InitClient(const char* ip, uint32_t port)
{
	mbServer = false;
	Socket* pSocket = InitSeNet();
	if (!pSocket->Connect(ip, port))
	{
		LOG_WARN("init client error, can not connect server....");
		// close connect event
		mEventCB(pSocket->GetFd(), SE_NET_EVENT_TIMEOUT, this);
		return false;
	}
	pSocket->SetSocketOptions();
	AddSession(pSocket);
	// connect event
	mEventCB(pSocket->GetFd(), SE_NET_EVENT_CONNECTED, this);
	mbServer = false;
	return true;
}

void SeNet::AddSession(Socket* pSocket)
{
	Session* pSession = g_pSessionPool->NewSession();
	if (pSession == nullptr) return;
	pSession->SetSocket(pSocket);
	if (!mbServer) 
		mSessions.emplace(0, pSession);
	else 
		mSessions.emplace(pSocket->GetFd(), pSession);
}

Session* SeNet::GetSession(socket_t fd)
{
	if (mSessions.empty()) return nullptr;
	if (!mbServer) { return mSessions[0]; }
	auto it = mSessions.find(fd);
	if (it != mSessions.end()) { return it->second; }
	return nullptr;
}

void SeNet::CloseSession(Session* pSession)
{
	socket_t fd = pSession->GetSocket()->GetFd();
	// close connect event
	mEventCB(fd, SE_NET_EVENT_EOF, this);
	mSessions.erase(fd);
	mEventOp->DelEvent(fd, EV_READ | EV_WRITE);
	g_pSessionPool->DelSession(pSession);
}

void SeNet::AcceptClient()
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
			pSocket->SetSocketOptions();
			AddSession(pSocket);
			mEventOp->SetMaxFd(connfd);
#ifdef SF_PLATFORM_WIN
			mEventOp->AddEvent(connfd, EV_READ);
#else
			mEventOp->AddEvent(connfd, EV_READ);
#endif
			mEventCB(connfd, SE_NET_EVENT_CONNECTED, this);
			LOG_INFO("accept client connect ...%d", connfd);
			continue;
		}
		break;
	}
}

void SeNet::EventWrite(Session* pSession)
{
	socket_t fd = pSession->GetSocket()->GetFd();
	//LOG_INFO("write event trigger....%d", fd);
	int nSendSize = 0;
	char* pSendBuf = pSession->GetSendBuf(nSendSize);
	while (pSession && pSendBuf && nSendSize >0)
	{
		int ret = ::send(fd, pSendBuf, nSendSize, 0);
		if (ret < 0)
		{
			int err = SocketGetError(fd);
			if (SOCKET_ERR_RW_RETRIABLE(err)) break;
			LOG_WARN("socket send error! err code %d, %s", errno, strerror(errno));
			CloseSession(pSession);
		}
		pSession->PostSendData(ret);
		nSendSize = 0;
		pSendBuf = pSession->GetSendBuf(nSendSize);
	}
	mEventOp->DelEvent(fd, EV_WRITE);
}

void SeNet::EventRead(Session* pSession)
{
	socket_t fd = pSession->GetSocket()->GetFd();
	//LOG_INFO("read event trigger....%d", fd);
	int nRecvSize = GetReadableSizeOnSocket(fd);
	int nRecvLeft = nRecvSize;
	for (; nRecvLeft >= 0;)
	{
		char* pRecvBuf = pSession->GetRecvBuf(nRecvLeft);
		int ret = ::recv(fd, pRecvBuf, nRecvLeft, 0);
		if (ret < 0)
		{
			int err = SocketGetError(fd);
			if (SOCKET_ERR_RW_RETRIABLE(err)) break;
			if (SOCKET_ERR_CONNECT_REFUSED(err))
			{
				LOG_WARN("connection peer closed! err code %d, %s", errno, strerror(errno));
				CloseSession(pSession);
				return;
			}
			LOG_WARN("socket recv error! err code %d, %s", errno, strerror(errno));
			CloseSession(pSession);
			return;
		}
		else if (ret == 0)  // read eof
		{
			LOG_WARN("connection peer closed! err code %d, %s", errno, strerror(errno));
			CloseSession(pSession);
			break;
		}
		nRecvLeft -= ret;
		pSession->PostRecvData(ret);

		for (;;)
		{
			// parse msg packet
			if (!Dismantle(pSession)) break;
		}
		if (nRecvLeft == 0) break;
	}
}

bool SeNet::Dismantle(Session* pSession)
{
	int nTotal = pSession->GetRecvTotal();
	if (nTotal > MSG_HEAD_LEN)
	{
		char headbuf[MSG_HEAD_LEN] = { 0 };
		pSession->ReadProtoHead(headbuf, MSG_HEAD_LEN);
		NetMsgHead MsgHead;
		MsgHead.DeCode(headbuf);
		int nMsgBodyLen = MsgHead.GetBodyLength();
		if (nTotal < (MSG_HEAD_LEN + nMsgBodyLen))
			return false;
		int nRead = nMsgBodyLen + MSG_HEAD_LEN;
		char *pMsgBuf = new char[nRead];
		pSession->Read(pMsgBuf, nRead);

		if (mRecvCB)
		{
			mRecvCB(pSession->GetSocket()->GetFd(), MsgHead.GetMsgID(), pMsgBuf + MSG_HEAD_LEN, MsgHead.GetBodyLength());
		}
		delete[] pMsgBuf;
		return true;
	}
	return false;
}

void SeNet::StartLoop(LOOP_RUN_TYPE run)
{
	while (!mbStop)
	{
		if (!mEventOp->Dispatch())
		{
			LOG_FATAL("eventloop dispatch error");
			break;
		}
		auto events = mEventOp->GetEvents();

		// do with events
		for (auto it = events.begin(); it != events.end(); it++)
		{
			if (mbServer && (it->second & EV_READ) && (mSocket->GetFd() == it->first))
			{
				AcceptClient();
				continue;
			}
			Session* pSession = GetSession(it->first);
			if (pSession == nullptr) continue;

			if (it->second & EV_READ)
			{
				EventRead(pSession);
			}
			if (it->second & EV_WRITE)
			{
				EventWrite(pSession);
			}
			if (it->second & EV_CLOSED)
			{
				LOG_INFO("close event trigger....%d", it->first);
				CloseSession(pSession);
			}
		}
		events.clear();
		if (run == LOOP_RUN_NONBLOCK) break;
	}
}

void SeNet::StopLoop()
{
	mEventOp->Clear();
	delete mEventOp;
	mEventOp = nullptr;
	mbStop = true;
	if (mSocket)
	{
		mSocket->CloseSocket();
		delete mSocket;
		mSocket = nullptr;
	}
	for (auto it : mSessions)
	{
		g_pSessionPool->DelSession(it.second);
	}
#ifdef SF_PLATFORM_WIN
	WSACleanup();
#endif
}

void SeNet::SendMsg(socket_t fd, const char* msg, int len)
{
	auto it = mSessions.find(fd);
	if (it != mSessions.end())
	{
		it->second->Send(msg, len);
		if(!mbServer) fd = it->second->GetSocket()->GetFd();
		mEventOp->AddEvent(fd, EV_WRITE);
	}
}

void SeNet::SendMsg(const char* msg, int len)
{
	for (auto& it : mSessions)
	{
		it.second->Send(msg, len);
		mEventOp->AddEvent(it.first, EV_WRITE);
	}
}

void SeNet::SendMsg(std::vector<socket_t>& fds, const char* msg, int len)
{
	for (auto& it : fds)
	{
		auto it2 = mSessions.find(it);
		if (it2 != mSessions.end())
		{
			it2->second->Send(msg, len);
			mEventOp->AddEvent(it2->first, EV_WRITE);
		}
	}
}

void SeNet::SendToAllClients(const char* msg, int len)
{
	if (!mbServer) return;

	for (auto& it : mSessions)
	{
		it.second->Send(msg, len);
		mEventOp->AddEvent(it.first, EV_WRITE);
	}
}

void SeNet::SendProtoMsg(socket_t fd, const int msg_id, const char* msg, int len)
{
	NetMsgHead MsgHead(msg_id, len);
	char pHead[MSG_HEAD_LEN] = { 0 };
	MsgHead.EnCode(pHead);
	SendMsg(fd, pHead, MSG_HEAD_LEN);
	SendMsg(fd, msg, len);
}

void SeNet::SendProtoMsg(std::vector<socket_t>& fdlist, const int msg_id, const char* msg, int len)
{
	NetMsgHead MsgHead(msg_id, len);
	char pHead[MSG_HEAD_LEN] = { 0 };
	MsgHead.EnCode(pHead);
	SendMsg(fdlist, pHead, MSG_HEAD_LEN);
	SendMsg(fdlist, msg, len);
}

void SeNet::SendProtoMsg(const int msg_id, const char* msg, int len)
{
	NetMsgHead MsgHead(msg_id, len);
	char pHead[MSG_HEAD_LEN] = { 0 };
	MsgHead.EnCode(pHead);
	SendMsg(pHead, MSG_HEAD_LEN);
	SendMsg(msg, len);
}

void SeNet::CloseClient(socket_t fd)
{
	for (auto& it : mSessions)
	{
		Session* pSession = it.second;
		if (pSession->GetSocket()->GetFd() == fd)
		{
			CloseSession(pSession);
			break;
		}
	}
}

void SeNet::CloseAllClient()
{
	for (auto it = mSessions.begin(); it != mSessions.end(); it++)
	{
		Session* pSession = it->second;
		CloseSession(pSession);
	}
	mSessions.clear();
}

