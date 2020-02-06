#include "SeFNetClient.h"
#include "LogUtil.h"
#include "Assertx.h"
#include "SeNet.h"
#include "SeFNet.h"

SeFNetClient::SeFNetClient()
{
	for (int eType = 0; eType < ServerType::SERVER_TYPE_MAX; ++eType)
	{
		CallBack cb;
		mmCallBack.emplace(eType, cb);
	}
}

SeFNetClient::~SeFNetClient()
{
	mmCallBack.clear();
}

void SeFNetClient::AddReceiveCallBack(ServerType eType, NET_RECEIVE_FUNCTOR_PTR functorPtr)
{
	auto it = mmCallBack.find(eType);
	if (it == mmCallBack.end()) return;

	mmCallBack[eType].mCallBackList.emplace_back(functorPtr);
}

void SeFNetClient::AddReceiveCallBack(ServerType eType, const int msg_id, NET_RECEIVE_FUNCTOR_PTR functorPtr)
{
	auto it = mmCallBack.find(eType);
	if (it == mmCallBack.end()) return;

	mmCallBack[eType].mReceiveCallBack.emplace(msg_id, functorPtr);
}

void SeFNetClient::AddEventCallBack(ServerType eType, NET_EVENT_FUNCTOR_PTR functorPtr)
{
	auto it = mmCallBack.find((int)eType);
	if (it == mmCallBack.end())
	{
		return;
	}
	mmCallBack[eType].mEventCallBackList.emplace_back(functorPtr);
}

void SeFNetClient::RemoveReceiveCallBack(ServerType eType, const int msg_id)
{
	auto it = mmCallBack.find(eType);
	if (it == mmCallBack.end()) return;

	mmCallBack[eType].mReceiveCallBack.erase(msg_id);
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
		ConnectState state = it.second->conn_state;
		switch (state)
		{
		case ConnectState::DISCONNECT:
			if (it.second->pNet->InitNet(it.second->ip.c_str(), it.second->port))
			{
				it.second->conn_state = ConnectState::NORMAL;
			}
			break;
		case ConnectState::NORMAL:
			it.second->pNet->Execute(run);
			break;
		case ConnectState::RECONNECT:
			it.second->conn_state = ConnectState::CONNECTING;
			it.second->pNet = std::make_shared<SeFNet>();
			InitCallBacks(it.second);
			if (it.second->pNet->InitNet(it.second->ip.c_str(), it.second->port))
			{
				it.second->conn_state = ConnectState::NORMAL;
			}
			else
			{
				it.second->conn_state = ConnectState::DISCONNECT;
			}
			break;
		default:
			break;
		}
	}
}

void SeFNetClient::ProcessAddConnect()
{
	if (mTemplist.empty()) return;

	for (auto& connPtr : mTemplist)
	{
		auto it = mConnecServers.find(connPtr->serv_id);
		if (it == mConnecServers.end())
		{
			connPtr->pNet = std::make_shared<SeFNet>();
			InitCallBacks(connPtr);
			Assert(connPtr->serv_id != 0);
			mConnecServers.emplace(connPtr->serv_id, connPtr);
			if (connPtr->pNet->InitNet(connPtr->ip.c_str(), connPtr->port))
			{
				connPtr->conn_state = ConnectState::NORMAL;
				CLOG_INFO << "connect to server " << connPtr->serv_name << " ok!!!" << CLOG_END;
			}
			else
			{
				connPtr->conn_state = ConnectState::DISCONNECT;
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

void SeFNetClient::SendByServId(int nServerId, const int msg_id, const char* msg, int len)
{
	auto it = mConnecServers.find(nServerId);
	if (it == mConnecServers.end()) return;

	it->second->pNet->SendMsg(0, msg_id, msg, len);
}
void SeFNetClient::SendByServIds(std::vector<int>& nServerIds, const int msg_id, const char* msg, int len)
{
	for (auto& it : nServerIds)
	{
		SendByServId(it, msg_id, msg, len);
	}
}
void SeFNetClient::SendPbByServId(int nServerId, const int msg_id, ::google::protobuf::Message* pb_msg)
{
	std::string strMsg = pb_msg->SerializeAsString();
	SendByServId(nServerId, msg_id, strMsg.c_str(), strMsg.length());
}
void SeFNetClient::SendPbByServIds(std::vector<int>& nServerIds, const int msg_id, ::google::protobuf::Message* pb_msg)
{
	for (auto& it : nServerIds)
	{
		std::string strMsg = pb_msg->SerializeAsString();
		SendByServId(it, msg_id, strMsg.c_str(), strMsg.length());
	}
}
void SeFNetClient::SendToAll(const int msg_id, const char* msg, int len)
{
	for (auto& it : mConnecServers)
	{
		if (it.second->pNet.get())
		{
			it.second->pNet->SendMsg(0, msg_id, msg, len);
		}
	}
}
void SeFNetClient::SendPBToAll(const int msg_id, ::google::protobuf::Message* pb_msg)
{
	for (auto& it : mConnecServers)
	{
		if (it.second->pNet.get())
		{
			std::string strMsg = pb_msg->SerializeAsString();
			it.second->pNet->SendMsg(0, msg_id, strMsg.c_str(), strMsg.length());
		}
	}
}

void SeFNetClient::SendByServType(ServerType type, const int msg_id, const char* msg, int len)
{

}
void SeFNetClient::SendByServTypes(std::vector<ServerType>& types, const int msg_id, const char* msg, int len)
{

}
void SeFNetClient::SendPbByServType(ServerType type, const int msg_id, ::google::protobuf::Message* pb_msg)
{

}
void SeFNetClient::SendPbByServTypes(std::vector<ServerType>& types, const int msg_id, ::google::protobuf::Message* pb_msg)
{

}

ConnectDataPtr SeFNetClient::GetServerNetInfo(const int& nServerID)
{
	auto it = mConnecServers.find(nServerID);
	if (it == mConnecServers.end()) return nullptr;

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

ConnectDataPtr SeFNetClient::GetServerNetInfo(const socket_t& sock_fd)
{
	for (auto it = mConnecServers.begin(); it != mConnecServers.end(); it++)
	{
		if (it->second->sock_fd == sock_fd)
			return it->second;
	}
	return nullptr;
}