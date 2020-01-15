#include "WorldNodeClient.h"
#include "JsonConfig.h"
#include "SeFNetClient.h"



void WorldNodeClient::InitHelper()
{
    mNetCliModule->AddEventCallBack(ServerType::SERVER_TYPE_MASTER, this, &WorldNodeClient::OnSocketEvent);
	mNetCliModule->AddEventCallBack(ServerType::SERVER_TYPE_GATE, this, &WorldNodeClient::OnSocketEvent);//world服连接master服务

	SetReportInfo();
	AddConnectServer();
}

void WorldNodeClient::SetReportInfo()
{
    mServerInfo.set_server_id(g_JsonConfig->m_ServerConf["NodeId"].asInt());
	mServerInfo.set_server_name(g_JsonConfig->m_ServerConf["NodeName"].asString());
	mServerInfo.set_server_cur_count(0);
	mServerInfo.set_server_ip(g_JsonConfig->m_ServerConf["NodeIp"].asString());
	mServerInfo.set_server_port(g_JsonConfig->m_ServerConf["NodePort"].asInt());
	mServerInfo.set_server_max_online(5000);
	mServerInfo.set_server_state(EServerState::EST_NORMAL);
	mServerInfo.set_server_type(ServerType::SERVER_TYPE_WORLD);
}

void WorldNodeClient::AddConnectServer()
{
	AddConnectMaster();
	mConnectType.emplace_back(SERVER_TYPE_GATE);
}

void WorldNodeClient::OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	OnSocketNodeEvent(nFd, nEvent, pNet);
}

