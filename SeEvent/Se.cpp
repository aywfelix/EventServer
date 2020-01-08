#include "Se.h"
#include "Socket.h"
#include "LogHelper.h"
#include "Session.h"
#include "LogHelper.h"
#include "SocketDefine.h"
#include "Session.h"
#include "MemPool.hpp"

#include "SeSelect.h"
#include "SeEpoll.h"
#include "SeFINet.h"


bool SeEventOp::Init()
{
	mMaxFd = INVALID_SOCKET;
	// init timeval
	gettimeofday(&mtv, nullptr);
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
	mActiveEvents[fd] = mask;
}

std::map<socket_t, int>& SeEventOp::GetActiveEvents()
{
	return mActiveEvents;
}

bool SeEventOp::Dispatch()
{
	return this->Dispatch(&mtv);
}


void SeNet::InitEventOp()
{
#ifdef _WIN32
	mEventOp = new SeSelect();
#else
	mEventOp = new SeEpoll();
#endif
	if (mEventOp == nullptr)
	{
		return;
	}
	mEventOp->Init();
#if defined SF_PLATFORM_WIN
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data)!=0)
	{
		AssertEx(0, "windows init socket api error");
		return;
	}
#endif
	mbStop = false;
	mSocket = new Socket;
	mSocket->CreateFd();
	mEventOp->SetMaxFd(mSocket->GetFd());
#ifdef _WIN32
	mEventOp->AddEvent(mSocket->GetFd(), EV_READ);
#else
	mEventOp->AddEvent(mSocket->GetFd(), EV_READ);
#endif
}

bool SeNet::InitServer(UINT port)
{
	InitEventOp();
	mbServer = true;
	if (!mSocket->Listen(port))
	{
		return false;
	}
	mSocket->SetSocketOptions();
	LOG_INFO("init server ok ....%d", mSocket->GetFd());
	return true;
}

bool SeNet::InitClient(const char* ip, UINT port)
{
	InitEventOp();
	if (!mSocket->Connect(ip, port))
	{
		LOG_WARN("init client error, can not connect server....");
		// close connect event
		mEventCB(mSocket->GetFd(), SE_NET_EVENT_TIMEOUT, this);
		return false;
	}
	mSocket->SetSocketOptions();
	AddSession(mSocket);
	LOG_INFO("init client ....%d", mSocket->GetFd());
	mbServer = false;
	return true;
}

void SeNet::AddSession(Socket* pSocket)
{
	Session* pSession = g_pSessionPool->NewSession();
	if (pSession == nullptr)
		return;
	pSession->SetSocket(pSocket);
	if (!mbServer)
	{
		mSessions.emplace(0, pSession);
	}
	else 
	{
		mSessions.emplace(pSocket->GetFd(), pSession);
	}
	// connect event
	mEventCB(pSocket->GetFd(), SE_NET_EVENT_CONNECTED, this);
}

Session* SeNet::GetSession(socket_t fd)
{
	if (mSessions.empty())
	{
		return nullptr;
	}
	if (!mbServer)
	{
		fd = 0;
	}
	auto it = mSessions.find(fd);
	if (it == mSessions.end())
	{
		return nullptr;
	}
	return it->second;
}

void SeNet::CloseSession(Session* pSession)
{
	socket_t fd = pSession->GetSocket()->GetFd();
	if (fd > 0)
	{
		// close connect event
		mEventCB(fd, SE_NET_EVENT_EOF, this);
		mSessions.erase(fd);
		mEventOp->DelEvent(fd, EV_READ | EV_WRITE);
		g_pSessionPool->DelSession(pSession);
	}
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
#ifdef _WIN32
			mEventOp->AddEvent(connfd, EV_READ);
#else
			mEventOp->AddEvent(connfd, EV_READ);
#endif
			LOG_INFO("accept client connect ...%d", connfd);
			continue;
		}
		break;
	}
}

void SeNet::EventRead(Session* pSession)
{
	socket_t fd = pSession->GetSocket()->GetFd();
	LOG_INFO("read event trigger....%d", fd);
	// if socket cache is readable then read data
	int nRecvSize = GetReadableSizeOnSocket(fd);
	int nRecvLeft = nRecvSize;
	LOG_INFO("can read data len==%d", nRecvSize);
	for (; nRecvLeft >= 0;)
	{
		char* pRecvBuf = pSession->GetRecvBuf(nRecvLeft);
		int ret = ::recv(fd, pRecvBuf, nRecvLeft, 0);
		if (ret < 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				LOG_INFO("socket recv data finish!");
				break;
			}
			else if (errno == 0)
			{
				break;
			}
			LOG_WARN("socket recv error! err code %d, %s", errno, strerror(errno));
			CloseSession(pSession);
			break;
		}
		else if (ret == 0)
		{
			LOG_WARN("connection peer closed! err code %d, %s", errno, strerror(errno));
			CloseSession(pSession);
			break;
		}
		nRecvLeft -= ret;
		if (ret > 0)
		{
			LOG_INFO("socket recv %d byte of content %s", ret, pRecvBuf);
		}
		pSession->PostRecvData(ret);

		for (;;)
		{
			// 进行数据解包
			if (!Dismantle(pSession))
			{
				break;
			}
		}
		if (nRecvLeft == 0)
		{
			break;
		}
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
		int nPacketLen = MsgHead.GetBodyLength();
		if (nTotal < (MSG_HEAD_LEN + nPacketLen))
			return false;
		int nRead = nPacketLen + MSG_HEAD_LEN;
		char *pMsgBuf = new char[nRead];
		pSession->Read(pMsgBuf, nRead);
		// 回调协议处理接口
		if (mRecvCB)
		{
			mRecvCB(pSession->GetSocket()->GetFd(), MsgHead.GetMsgID(), pMsgBuf + MSG_HEAD_LEN, MsgHead.GetBodyLength());
		}
		delete[] pMsgBuf;
		return true;
	}
	return false;
}

void SeNet::EventWrite(Session* pSession)
{
	socket_t fd = pSession->GetSocket()->GetFd();
	LOG_INFO("write event trigger....%d", fd);
	int nSendSize = 0;
	char* pSendBuf = pSession->GetSendBuf(nSendSize);
	while (pSession && pSendBuf)
	{
		int ret = ::send(fd, pSendBuf, nSendSize, 0);
		if (ret <= 0)
		{
			break;
		}
		pSession->PostSendData(ret);
		LOG_INFO("socket write to socket....%d len, content %s", ret, pSendBuf);
		nSendSize = 0;
		pSendBuf = pSession->GetSendBuf(nSendSize);
	}
	mEventOp->DelEvent(fd, EV_WRITE);
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
		auto activemq = mEventOp->GetActiveEvents();

		// do with activemq
		for (auto it = activemq.begin(); it != activemq.end(); it++)
		{
			if (mbServer && (it->second & EV_READ) && (mSocket->GetFd() == it->first))
			{
				AcceptClient();
				continue;
			}
			Session* pSession = GetSession(it->first);
			if (pSession == nullptr)
			{
				LOG_WARN("eventloop session is null, socket==%d", it->first);
				continue;
			}
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
		activemq.clear();
		if (run == LOOP_RUN_NONBLOCK)
		{
			break;
		}
	}
}

void SeNet::StopLoop()
{
	mEventOp->Clear();
	mbStop = true;
	mSocket->CloseSocket();
	for (auto it : mSessions)
	{
		g_pSessionPool->DelSession(it.second);
	}
}

void SeNet::SendMsg(socket_t fd, const char* msg, int len)
{
	auto it = mSessions.find(fd);
	if (it != mSessions.end())
	{
		it->second->Send(msg, len);
		if(!mbServer)
			fd = it->second->GetSocket()->GetFd();
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

void SeNet::SendMsg(std::vector<socket_t>& fdlist, const char* msg, int len)
{
	for (auto& it : fdlist)
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
	if (!mbServer)
	{
		return;
	}
	for (auto& it : mSessions)
	{
		it.second->Send(msg, len);
		mEventOp->AddEvent(it.first, EV_WRITE);
	}
}

void SeNet::SendProtoMsg(socket_t fd, const int nMsgID, const char* msg, int len)
{
	NetMsgHead MsgHead(nMsgID, len);
	int nSend = len + MSG_HEAD_LEN;
	char pHead[MSG_HEAD_LEN] = { 0 };
	MsgHead.EnCode(pHead);
	SendMsg(fd, pHead, MSG_HEAD_LEN);
	SendMsg(fd, msg, len);
}

void SeNet::SendProtoMsg(std::vector<socket_t>& fdlist, const int nMsgID, const char* msg, int len)
{
	NetMsgHead MsgHead(nMsgID, len);
	int nSend = len + MSG_HEAD_LEN;
	char pHead[MSG_HEAD_LEN] = { 0 };
	MsgHead.EnCode(pHead);
	SendMsg(fdlist, pHead, MSG_HEAD_LEN);
	SendMsg(fdlist, msg, len);
}

void SeNet::SendProtoMsg(const int nMsgID, const char* msg, int len)
{
	NetMsgHead MsgHead(nMsgID, len);
	int nSend = len + MSG_HEAD_LEN;
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

bool SeNet::ReceivePB(const int nMsgID, const std::string& strMsg, google::protobuf::Message* pMsg)
{
	return ReceivePB(nMsgID, strMsg.c_str(), strMsg.length(), pMsg);
}

bool SeNet::ReceivePB(const int nMsgID, const char* msg, const UINT32 nLen, google::protobuf::Message* pData)
{
	if (msg == nullptr)
	{
		return false;
	}
	pData->ParseFromArray(msg, nLen);
	return true;
}