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

void SeFNetClient::OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
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

void SeFNetClient::OnSocketConnect(const socket_t nFd, SeNet* pNet)
{
	LOG_INFO("connect server ok, socket(%d)", nFd);
}

void SeFNetClient::OnSocketDisConnect(const socket_t nFd, SeNet* pNet)
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
	mmCallBack[eType].mEventCallBackList.emplace_back(functorPtr);
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

void SeFNetClient::Excute(LOOP_RUN_TYPE run)
{
	ProcessExcute(run);
	ProcessAddConnect();
}

void SeFNetClient::ProcessExcute(LOOP_RUN_TYPE run)
{
	for (auto& it : mConnecServers)
	{
		ConnectState state = it.second.ConnState;
		switch (state)
		{
		case ConnectState::DISCONNECT:
			if (it.second.pNet->InitNet(it.second.Ip.c_str(), it.second.Port))
			{
				it.second.ConnState = ConnectState::NORMAL;
				InitCallBacks(it.second);
			}
			break;
		case ConnectState::NORMAL:
			it.second.pNet->Excute(run);
			break;
		case ConnectState::RECONNECT:
			it.second.ConnState = ConnectState::CONNECTING;
			it.second.pNet = std::make_shared<SeFNet>();
			if (it.second.pNet->InitNet(it.second.Ip.c_str(), it.second.Port))
			{
				it.second.ConnState = ConnectState::NORMAL;
				InitCallBacks(it.second);
			}
			else
			{
				it.second.ConnState = ConnectState::DISCONNECT;
			}
			break;
		default:
			break;
		}
	}
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
			conn.pNet = std::make_shared<SeFNet>();
			if (conn.pNet->InitNet(conn.Ip.c_str(), conn.Port))
			{
				conn.ConnState = ConnectState::NORMAL;
				InitCallBacks(conn);
			}
			else
			{
				conn.ConnState = ConnectState::DISCONNECT;
			}
			mConnecServers.emplace(conn.ServerId, conn);
		}
	}
	mTemplist.clear();
}

void SeFNetClient::InitCallBacks(ConnectData& data)
{
	for (auto& it : mmCallBack)
	{
		for (auto& receivecb : it.second.mReceiveCallBack)
		{
			data.pNet->AddReceiveCallBack(receivecb.first, receivecb.second);
		}
		for (auto& receivecb : it.second.mCallBackList)
		{
			data.pNet->AddReceiveCallBack(receivecb);
		}
		for (auto& eventcb : it.second.mEventCallBackList)
		{
			data.pNet->AddEventCallBack(eventcb);
		}
	}
}

void SeFNetClient::SendByServerId(int nServerId, const int nMsgID, const char* msg, int len)
{
	auto it = mConnecServers.find(nServerId);
	if (it == mConnecServers.end())
	{
		return;
	}
	it->second.pNet->SendMsg(0, nMsgID, msg, len);
}
void SeFNetClient::SendByServerIds(std::vector<int>& nServerIds, const int nMsgID, const char* msg, int len)
{
	for (auto& it : nServerIds)
	{
		SendByServerId(it, nMsgID, msg, len);
	}
}
void SeFNetClient::SendProtoBuffer(int nServerId, const int nMsgID, ::google::protobuf::Message* pMsg)
{
	std::string strMsg = pMsg->SerializeAsString();
	SendByServerId(nServerId, nMsgID, strMsg.c_str(), strMsg.length());
}
void SeFNetClient::SendProtoBuffer(std::vector<int>& nServerIds, const int nMsgID, ::google::protobuf::Message* pMsg)
{
	for (auto& it : nServerIds)
	{
		std::string strMsg = pMsg->SerializeAsString();
		SendByServerId(it, nMsgID, strMsg.c_str(), strMsg.length());
	}
}
void SeFNetClient::SendToAll(const int nMsgID, const char* msg, int len)
{
	for (auto& it : mConnecServers)
	{
		if (it.second.pNet.get())
		{
			it.second.pNet->SendMsg(0, nMsgID, msg, len);
		}
	}
}
void SeFNetClient::SendToAll(const int nMsgID, ::google::protobuf::Message* pMsg)
{
	for (auto& it : mConnecServers)
	{
		if (it.second.pNet.get())
		{
			std::string strMsg = pMsg->SerializeAsString();
			it.second.pNet->SendMsg(0, nMsgID, strMsg.c_str(), strMsg.length());
		}
	}
}