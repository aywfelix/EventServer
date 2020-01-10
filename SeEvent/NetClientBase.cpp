#include "NetClientBase.h"
#include "SeFNodeNet.pb.h"
#include "LogHelper.h"
#include "SeFNetClient.h"
#include "JsonConfig.h"

bool NetClientBase::Init()
{
	mNetCliModule = new SeFNetClient();
	mNetCliModule->AddReceiveCallBack(EServerType::SERVER_TYPE_MASTER, MASTER_REPORT_SERVER_INFO_TO_SERVER, this, &NetClientBase::OnMasterMessage);
	
	return true;
}

ConnectDataPtr NetClientBase::GetServerNetInfo(const int& nServerID)
{
	return mNetCliModule->GetServerNetInfo(nServerID);
}

ConnectDataPtr NetClientBase::GetServerNetInfo(const SeNet* pNet)
{
	return mNetCliModule->GetServerNetInfo(pNet);
}

ConnectDataPtr NetClientBase::GetServerNetInfo(const socket_t& nFd)
{
	return mNetCliModule->GetServerNetInfo(nFd);
}

void NetClientBase::OnSocketNodeEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	ConnectDataPtr pConnData = mNetCliModule->GetServerNetInfo(pNet);
	if (nEvent == SE_NET_EVENT_CONNECTED && pConnData)
	{
		pConnData->SockFd = nFd;
		mNetCliModule->SendPbByServId(pConnData->ServerId, REPORT_CLIENT_INFO_TO_SERVER, &mServerInfo);
		LOG_INFO("(%d : %s) report info to %s server", mServerInfo.server_id(), mServerInfo.server_name().c_str(), (pConnData->name).c_str());
	}
	else
	{
		LOG_ERR("%s server client disconnected!!!", mServerInfo.server_name().c_str());
	}
}

void NetClientBase::OnMasterMessage(const socket_t nFd, const int nMsgID, const char* msg, const UINT32 nLen)
{
	if (mConnectType.empty()) return;
	ServerReportList report_list;
	if (!report_list.ParseFromArray(msg, nLen))
	{
		LOG_ERR("report list parse error.....");
		return;
	}

	for (int i = 0; i < report_list.server_info_size(); ++i)
	{
		const ServerReport& server_info = report_list.server_info(i);
		for (auto& serverType : mConnectType)
		{
			if (server_info.server_type() == serverType)
			{
				ConnectDataPtr ServerData = std::make_shared<ConnectData>();
				ServerData->ServerId = server_info.server_id();
				ServerData->Port = server_info.server_port();
				ServerData->name = server_info.server_name();
				ServerData->Ip = server_info.server_ip();
				ServerData->ServerType = EServerType(server_info.server_type());
				ServerData->ConnState = ConnectState::CONNECTING;
				mNetCliModule->AddServer(ServerData);
			}
		}
	}
}


void NetClientBase::Loop()
{
	mNetCliModule->Execute(LOOP_RUN_NONBLOCK);
}


void NetClientBase::AddConnectMaster()
{
	if (this->GetServerType() != EServerType::SERVER_TYPE_MASTER)
	{
		ConnectDataPtr ServerData = std::make_shared<ConnectData>();
		ServerData->ServerId = g_JsonConfig->m_Root["MasterServer"]["NodeId"].asInt();
		ServerData->ServerType = EServerType::SERVER_TYPE_MASTER;
		ServerData->Ip = g_JsonConfig->m_Root["MasterServer"]["NodeIp"].asString();
		ServerData->Port = g_JsonConfig->m_Root["MasterServer"]["NodePort"].asInt();
		ServerData->name = g_JsonConfig->m_Root["MasterServer"]["NodeName"].asString();
		ServerData->ConnState = ConnectState::CONNECTING;
		mNetCliModule->AddServer(ServerData);
	}
}
