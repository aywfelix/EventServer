#include "SeFNetClient.h"
#include "LogHelper.h"


SeFNetClient::SeFNetClient()
{
	for (int eType = 0; eType < EServerType::SERVER_TYPE_MAX; ++eType)
	{
		CallBack cb;
		mmCallBack.emplace(eType, cb);
	}
}

SeFNetClient::~SeFNetClient()
{
	mmCallBack.clear();
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

void SeFNetClient::AddServer(ConnectDataPtr& info)
{
	mTemplist.emplace_back(info);
}

void SeFNetClient::Execute(LOOP_RUN_TYPE run)
{
	ProcessExecute(run);
	ProcessAddConnect();
}

void SeFNetClient::ProcessExecute(LOOP_RUN_TYPE run)
{
	for (auto& it : mConnecServers)
	{
		ConnectState state = it.second->ConnState;
		switch (state)
		{
		case ConnectState::DISCONNECT:
			if (it.second->pNet->InitNet(it.second->Ip.c_str(), it.second->Port))
			{
				it.second->ConnState = ConnectState::NORMAL;
			}
			break;
		case ConnectState::NORMAL:
			it.second->pNet->Execute(run);
			break;
		case ConnectState::RECONNECT:
			it.second->ConnState = ConnectState::CONNECTING;
			it.second->pNet = std::make_shared<SeFNet>();
			InitCallBacks(it.second);
			if (it.second->pNet->InitNet(it.second->Ip.c_str(), it.second->Port))
			{
				it.second->ConnState = ConnectState::NORMAL;
			}
			else
			{
				it.second->ConnState = ConnectState::DISCONNECT;
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

	for (auto& connPtr : mTemplist)
	{
		auto it = mConnecServers.find(connPtr->ServerId);
		if (it == mConnecServers.end())
		{
			connPtr->pNet = std::make_shared<SeFNet>();
			InitCallBacks(connPtr);
			Assert(connPtr->ServerId != 0);
			mConnecServers.emplace(connPtr->ServerId, connPtr);
			if (connPtr->pNet->InitNet(connPtr->Ip.c_str(), connPtr->Port))
			{
				connPtr->ConnState = ConnectState::NORMAL;
				CLOG_INFO << "connect to server " << connPtr->name << " ok!!!" << CLOG_END;
			}
			else
			{
				connPtr->ConnState = ConnectState::DISCONNECT;
			}
		}
	}
	mTemplist.clear();
}

void SeFNetClient::InitCallBacks(ConnectDataPtr& data)
{
	for (auto& it : mmCallBack)
	{
		for (auto& receivecb : it.second.mReceiveCallBack)
		{
			data->pNet->AddReceiveCallBack(receivecb.first, receivecb.second);
		}
		for (auto& receivecb : it.second.mCallBackList)
		{
			data->pNet->AddReceiveCallBack(receivecb);
		}
		for (auto& eventcb : it.second.mEventCallBackList)
		{
			data->pNet->AddEventCallBack(eventcb);
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
	it->second->pNet->SendMsg(0, nMsgID, msg, len);
}
void SeFNetClient::SendByServerIds(std::vector<int>& nServerIds, const int nMsgID, const char* msg, int len)
{
	for (auto& it : nServerIds)
	{
		SendByServerId(it, nMsgID, msg, len);
	}
}
void SeFNetClient::SendPbByServId(int nServerId, const int nMsgID, ::google::protobuf::Message* pMsg)
{
	std::string strMsg = pMsg->SerializeAsString();
	SendByServerId(nServerId, nMsgID, strMsg.c_str(), strMsg.length());
}
void SeFNetClient::SendPbByServIds(std::vector<int>& nServerIds, const int nMsgID, ::google::protobuf::Message* pMsg)
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
		if (it.second->pNet.get())
		{
			it.second->pNet->SendMsg(0, nMsgID, msg, len);
		}
	}
}
void SeFNetClient::SendPBToAll(const int nMsgID, ::google::protobuf::Message* pMsg)
{
	for (auto& it : mConnecServers)
	{
		if (it.second->pNet.get())
		{
			std::string strMsg = pMsg->SerializeAsString();
			it.second->pNet->SendMsg(0, nMsgID, strMsg.c_str(), strMsg.length());
		}
	}
}

ConnectDataPtr SeFNetClient::GetServerNetInfo(const int& nServerID)
{
	auto it = mConnecServers.find(nServerID);
	if (it == mConnecServers.end())
	{
		return nullptr;
	}

	return it->second;
}

ConnectDataPtr SeFNetClient::GetServerNetInfo(const SeNet* pNet)
{
	for (auto it = mConnecServers.begin();it != mConnecServers.end(); it++)
	{
		if (it->second->pNet->GetNet() == pNet)
		{
			return it->second;
		}
	}
	return nullptr;
}

ConnectDataPtr SeFNetClient::GetServerNetInfo(const socket_t& nFd)
{
	for (auto it = mConnecServers.begin(); it != mConnecServers.end(); it++)
	{
		if (it->second->SockFd == nFd)
			return it->second;
	}
	return nullptr;
}