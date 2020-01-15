#include "DBNodeClient.h"
#include "JsonConfig.h"
#include "SeFNetClient.h"



void DBNodeClient::InitHelper()
{
	mNetCliModule->AddEventCallBack(ServerType::SERVER_TYPE_MASTER, this, &DBNodeClient::OnSocketEvent);
	mNetCliModule->AddEventCallBack(ServerType::SERVER_TYPE_GATE, this, &DBNodeClient::OnSocketEvent); // world服连接master服务

	SetReportInfo();
	AddConnectServer();
}

void DBNodeClient::SetReportInfo()
{
	mServerInfo.set_server_id(g_JsonConfig->m_ServerConf["NodeId"].asInt());
	mServerInfo.set_server_name(g_JsonConfig->m_ServerConf["NodeName"].asString());
	mServerInfo.set_server_cur_count(0);
	mServerInfo.set_server_ip(g_JsonConfig->m_ServerConf["NodeIp"].asString());
	mServerInfo.set_server_port(g_JsonConfig->m_ServerConf["NodePort"].asInt());
	mServerInfo.set_server_max_online(5000);
	mServerInfo.set_server_state(EServerState::EST_NORMAL);
	mServerInfo.set_server_type(ServerType::SERVER_TYPE_DB);
}

void DBNodeClient::AddConnectServer()
{
	AddConnectMaster();
	mConnectType.push_back(SERVER_TYPE_GATE);
}

void DBNodeClient::OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
    OnSocketNodeEvent(nFd, nEvent, pNet);
}

