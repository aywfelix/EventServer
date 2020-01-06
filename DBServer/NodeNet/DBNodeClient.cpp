#include "DBNodeClient.h"
#include "JsonConfig.h"
#include "SeFNetClient.h"

using namespace SeFNetProto;

void DBNodeClient::InitHelper()
{
	mpNetClientModule->AddEventCallBack(EServerType::SERVER_TYPE_MASTER, this, &DBNodeClient::OnSocketEvent);
	mpNetClientModule->AddEventCallBack(EServerType::SERVER_TYPE_GATE, this, &DBNodeClient::OnSocketEvent); // world服连接master服务

	SetServerInfoReport();
	AddConnectServer();
}

void DBNodeClient::SetServerInfoReport()
{
	mServerReport.set_server_id(g_pJsonConfig->m_ServerConf["NodeId"].asInt());
	mServerReport.set_server_name(g_pJsonConfig->m_ServerConf["NodeName"].asString());
	mServerReport.set_server_cur_count(0);
	mServerReport.set_server_ip(g_pJsonConfig->m_ServerConf["NodeIp"].asString());
	mServerReport.set_server_port(g_pJsonConfig->m_ServerConf["NodePort"].asInt());
	mServerReport.set_server_max_online(5000);
	mServerReport.set_server_state(EServerState::EST_NORMAL);
	mServerReport.set_server_type(EServerType::SERVER_TYPE_DB);
}

void DBNodeClient::AddConnectServer()
{
	AddConnectMaster();
	mConnectType.emplace_back(SERVER_TYPE_GATE);
}

void DBNodeClient::OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
    OnSocketNodeEvent(nFd, nEvent, pNet);
}

