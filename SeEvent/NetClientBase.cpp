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
			int nServerId = pServerData->ServerId;
			mpNetClientModule->SendPBByServerId(nServerId, REPORT_CLIENT_INFO_TO_SERVER, &mServerReport);
			LOG_INFO("%s server connect to %s server ok", mServerReport.server_name(), pServerData->name);
		}
	}
	else
	{
		LOG_ERR("connect close!!!");
		if (pServerData)
		{
			LOG_ERR("%s server connect to %s server failed", mServerReport.server_name(), pServerData->name);
		}
	}
}

void NetClientBase::OnMasterMessage(const socket_t nFd, const int nMsgID, const char* msg, const UINT32 nLen)
{
	ServerReportList* report_list = nullptr;
	report_list->ParseFromArray(msg, nLen);
	if (report_list == nullptr)
	{
		return;
	}
	ConnectData xServerData;
	for (int i = 0; i < report_list->server_info_size(); ++i)
	{
		const ServerReport& server_info = report_list->server_info(i);
		for (auto serverType : mConnectType)
		{
			if (server_info.server_type() == serverType)
			{
				xServerData.ServerId = server_info.server_id();
				xServerData.Port = server_info.server_port();
				xServerData.name = server_info.server_name();
				xServerData.Ip = server_info.server_ip();
				xServerData.ServerType = EServerType(server_info.server_type());
				mpNetClientModule->AddServer(xServerData);
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
		ConnectData xServerData;
		xServerData.ServerId = g_pJsonConfig->m_Root["MasterServer"]["NodeID"].asInt();
		xServerData.ServerType = EServerType::SERVER_TYPE_MASTER;
		xServerData.Ip = g_pJsonConfig->m_Root["MasterServer"]["NodeIp"].asString();
		xServerData.Port = g_pJsonConfig->m_Root["MasterServer"]["NodePort"].asInt();
		xServerData.name = g_pJsonConfig->m_Root["MasterServer"]["NodeName"].asString();
		mpNetClientModule->AddServer(xServerData);
	}
}
