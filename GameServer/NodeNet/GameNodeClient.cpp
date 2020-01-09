#include "GameNodeClient.h"
#include "JsonConfig.h"
#include "SeFNetClient.h"

using namespace SeFNetProto;

void GameNodeClient::InitHelper()
{
    mpNetClientModule->AddEventCallBack(EServerType::SERVER_TYPE_MASTER, this, &GameNodeClient::OnSocketEvent);
	mpNetClientModule->AddEventCallBack(EServerType::SERVER_TYPE_WORLD, this, &GameNodeClient::OnSocketEvent);
	mpNetClientModule->AddEventCallBack(EServerType::SERVER_TYPE_GATE, this, &GameNodeClient::OnSocketEvent);
	SetServerInfoReport();
	AddConnectServer();
}

void GameNodeClient::SetServerInfoReport()
{
	mServerReport.set_server_id(g_pJsonConfig->m_ServerConf["NodeId"].asInt());
	mServerReport.set_server_name(g_pJsonConfig->m_ServerConf["NodeName"].asString());
	mServerReport.set_server_cur_count(0);
	mServerReport.set_server_ip(g_pJsonConfig->m_ServerConf["NodeIp"].asString());
	mServerReport.set_server_port(g_pJsonConfig->m_ServerConf["NodePort"].asInt());
	mServerReport.set_server_max_online(2000);
	mServerReport.set_server_state(EServerState::EST_NORMAL);
	mServerReport.set_server_type(EServerType::SERVER_TYPE_GAME);
}

void GameNodeClient::AddConnectServer()
{
	AddConnectMaster();
	mConnectType.push_back(EServerType::SERVER_TYPE_WORLD);
	mConnectType.push_back(EServerType::SERVER_TYPE_GATE);
}


void GameNodeClient::OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
    OnSocketNodeEvent(nFd, nEvent, pNet);
}