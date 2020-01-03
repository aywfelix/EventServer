#include "GateNodeClient.h"
#include "JsonConfig.h"
#include "Se.h"
#include "SeFNetClient.h"

void GateNodeClient::InitHelper()
{
	mpNetClientModule->AddEventCallBack(EServerType::SERVER_TYPE_MASTER, this, &GateNodeClient::OnSocketEvent);
	SetServerInfoReport();
	AddConnectServer();
}

void GateNodeClient::SetServerInfoReport()
{
	mServerReport.set_server_id(g_pJsonConfig->m_ServerConf["NodeId"].asInt());
	mServerReport.set_server_name(g_pJsonConfig->m_ServerConf["NodeName"].asString());
	mServerReport.set_server_cur_count(0);
	mServerReport.set_server_ip(g_pJsonConfig->m_ServerConf["NodeIp"].asString());
	mServerReport.set_server_port(g_pJsonConfig->m_ServerConf["NodePort"].asInt());
	mServerReport.set_server_max_online(2000);
	mServerReport.set_server_state(SeFNetProto::EServerState::EST_NORMAL);
	mServerReport.set_server_type(SERVER_TYPE_GATE);
}

void GateNodeClient::AddConnectServer()
{
	AddConnectMaster();
}

void GateNodeClient::OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
    OnSocketNodeEvent(nFd, nEvent, pNet);
}



