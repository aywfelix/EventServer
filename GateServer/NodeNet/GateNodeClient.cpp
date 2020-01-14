#include "GateNodeClient.h"
#include "JsonConfig.h"
#include "SeNet.h"
#include "SeFNetClient.h"

void GateNodeClient::InitHelper()
{
	mNetCliModule->AddEventCallBack(EServerType::SERVER_TYPE_MASTER, this, &GateNodeClient::OnSocketEvent);
	SetReportInfo();
	AddConnectServer();
}

void GateNodeClient::SetReportInfo()
{
	mServerInfo.set_server_id(g_JsonConfig->m_ServerConf["NodeId"].asInt());
	mServerInfo.set_server_name(g_JsonConfig->m_ServerConf["NodeName"].asString());
	mServerInfo.set_server_cur_count(0);
	mServerInfo.set_server_ip(g_JsonConfig->m_ServerConf["NodeIp"].asString());
	mServerInfo.set_server_port(g_JsonConfig->m_ServerConf["NodePort"].asInt());
	mServerInfo.set_server_max_online(2000);
	mServerInfo.set_server_state(EServerState::EST_NORMAL);
	mServerInfo.set_server_type(EServerType::SERVER_TYPE_GATE);
}

void GateNodeClient::AddConnectServer()
{
	AddConnectMaster();
}

void GateNodeClient::OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
    OnSocketNodeEvent(nFd, nEvent, pNet);
}



