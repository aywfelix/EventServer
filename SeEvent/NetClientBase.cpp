#include "NetClientBase.h"
#include "SeFNodeNet.pb.h"
#include "LogHelper.h"
#include "SeFNetClient.h"
#include "JsonConfig.h"

using namespace SeFNetProto;

bool NetClientBase::Init()
{
	mpNetClientModule = new SeFNetClient();
	mpNetClientModule->AddReceiveCallBack(EServerType::SERVER_TYPE_MASTER, MASTER_REPORT_SERVER_INFO_TO_SERVER, this, &NetClientBase::OnMasterMessage);
	
	return true;
}

ConnectDataPtr NetClientBase::GetServerNetInfo(const int nServerID)
{
	return mpNetClientModule->GetServerNetInfo(nServerID);
}

ConnectDataPtr NetClientBase::GetServerNetInfo(const SeNet* pNet)
{
	return mpNetClientModule->GetServerNetInfo(pNet);
}

void NetClientBase::OnSocketNodeEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	ConnectDataPtr pServerData = mpNetClientModule->GetServerNetInfo(pNet);
	if (nEvent == SE_NET_EVENT_CONNECTED)
	{
		if (pServerData)
		{
			pServerData->SockFd = nFd;
			int nServerId = pServerData->ServerId;
			mpNetClientModule->SendPBByServerId(nServerId, REPORT_CLIENT_INFO_TO_SERVER, &mServerReport);
			LOG_INFO("%s server connect to %s server ok", mServerReport.server_name().c_str(), (pServerData->name).c_str());
		}
	}
	else
	{
		LOG_ERR("connect close!!!");
		if (pServerData)
		{
			LOG_ERR("%s server connect to %s server failed", mServerReport.server_name().c_str(), (pServerData->name).c_str());
		}
	}
}

void NetClientBase::OnMasterMessage(const socket_t nFd, const int nMsgID, const char* msg, const UINT32 nLen)
{
	ServerReportList report_list;
	report_list.ParseFromArray(msg, nLen);
	if (report_list.server_info_size() == 0)
	{
		return;
	}
	ConnectDataPtr ServerData = std::make_shared<ConnectData>();
	for (int i = 0; i < report_list.server_info_size(); ++i)
	{
		const ServerReport& server_info = report_list.server_info(i);
		for (auto serverType : mConnectType)
		{
			if (server_info.server_type() == serverType)
			{
				ServerData->ServerId = server_info.server_id();
				ServerData->Port = server_info.server_port();
				ServerData->name = server_info.server_name();
				ServerData->Ip = server_info.server_ip();
				ServerData->ServerType = EServerType(server_info.server_type());
				mpNetClientModule->AddServer(ServerData);
			}
		}
	}
}


void NetClientBase::Loop()
{
	mpNetClientModule->Execute(LOOP_RUN_NONBLOCK);
}


void NetClientBase::AddConnectMaster()
{
	if (this->GetServerType() != EServerType::SERVER_TYPE_MASTER)
	{
		ConnectDataPtr ServerData = std::make_shared<ConnectData>();
		ServerData->ServerId = g_pJsonConfig->m_Root["MasterServer"]["NodeId"].asInt();
		ServerData->ServerType = EServerType::SERVER_TYPE_MASTER;
		ServerData->Ip = g_pJsonConfig->m_Root["MasterServer"]["NodeIp"].asString();
		ServerData->Port = g_pJsonConfig->m_Root["MasterServer"]["NodePort"].asInt();
		ServerData->name = g_pJsonConfig->m_Root["MasterServer"]["NodeName"].asString();
		mpNetClientModule->AddServer(ServerData);
	}
}
