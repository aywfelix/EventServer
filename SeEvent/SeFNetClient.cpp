#include "SeFNetClient.h"
#include "LogHelper.h"

void SeFNetClient::Init()
{
	for (int eType=0; eType <EServerType::SERVER_TYPE_MAX;++eType)
	{
		CallBack cb;
		mmCallBack.emplace(eType, cb);
	}
	for (int i = 0; i < SERVER_TYPE_MAX; ++i)
	{
		AddEventCallBack((EServerType)i, this, &SeFNetClient::OnSocketEvent);
	}
}

void SeFNetClient::OnSocketEvent(const SOCK nFd, const SE_NET_EVENT nEvent, seEventLoop* pNet)
{
	if (nEvent & SE_NET_EVENT_CONNECTED)
	{
		OnSocketConnect(nFd, pNet);
	}
	else
	{
		OnSocketDisConnect(nFd, pNet);
	}
}

void SeFNetClient::OnSocketConnect(const SOCK nFd, seEventLoop* pNet)
{
	LOG_INFO("connect server ok, socket(%d)", nFd);
}

void SeFNetClient::OnSocketDisConnect(const SOCK nFd, seEventLoop* pNet)
{
	LOG_WARN("disconnect, socket(%d)", nFd);
}

void SeFNetClient::AddReceiveCallBack(EServerType eType, NET_RECEIVE_FUNCTOR_PTR functorPtr)
{
	auto it = mmCallBack.find(eType);
	if (it == mmCallBack.end())
	{
		return;
	}
	mmCallBack[eType].mCallBackList.emplace_back(functorPtr);
}

void SeFNetClient::AddReceiveCallBack(EServerType eType, UINT32 nMsgId, NET_RECEIVE_FUNCTOR_PTR functorPtr)
{
	auto it = mmCallBack.find(eType);
	if (it == mmCallBack.end())
	{
		return;
	}
	mmCallBack[eType].mReceiveCallBack.emplace(nMsgId, functorPtr);
}

void SeFNetClient::AddEventCallBack(EServerType eType, NET_EVENT_FUNCTOR_PTR functorPtr)
{
	auto it = mmCallBack.find((int)eType);
	if (it == mmCallBack.end())
	{
		return;
	}
	mmCallBack[eType].mEventCallBack.emplace_back(functorPtr);
}

void SeFNetClient::RemoveReceiveCallBack(EServerType eType, UINT32 nMsgId)
{
	auto it = mmCallBack.find(eType);
	if (it == mmCallBack.end())
	{
		return;
	}
	mmCallBack[eType].mReceiveCallBack.erase(nMsgId);
}

void SeFNetClient::AddServer(const ConnectData& info)
{
	mTemplist.emplace_back(info);
}

void SeFNetClient::Excute()
{
	ProcessExcute();
	ProcessAddConnect();
}

void SeFNetClient::ProcessExcute()
{

}

void SeFNetClient::ProcessAddConnect()
{
	if (mTemplist.empty())
	{
		return;
	}

	for (auto conn : mTemplist)
	{
		auto it = mConnecServers.find(conn.ServerId);
		if (it == mConnecServers.end())
		{
			conn.ConnState = ConnectState::CONNECTING;
			conn.pNet = std::make_shared<seEventLoop>();
			conn.pNet->Init();
			if (conn.pNet->InitClient(conn.Ip.c_str(), conn.Port))
			{
				conn.ConnState = ConnectState::NORMAL;
				InitCallBacks(conn);
			}
			else
			{
				conn.ConnState = ConnectState::DISCONNECT;
			}
			mConnecServers.emplace(data.ServerId, conn);
		}
	}
	mTemplist.clear();
}

void SeFNetClient::InitCallBacks(ConnectData& data)
{

}