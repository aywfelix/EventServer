#include "GameNodeClient.h"
#include "JsonConfig.h"
#include "SeFNetClient.h"



void GameNodeClient::InitHelper()
{
    mNetCliModule->AddEventCallBack(EServerType::SERVER_TYPE_MASTER, this, &GameNodeClient::OnSocketEvent);
	mNetCliModule->AddEventCallBack(EServerType::SERVER_TYPE_WORLD, this, &GameNodeClient::OnSocketEvent);
	mNetCliModule->AddEventCallBack(EServerType::SERVER_TYPE_GATE, this, &GameNodeClient::OnSocketEvent);
	SetReportInfo();
	AddConnectServer();
}

void GameNodeClient::SetReportInfo()
{
	mServerInfo.set_server_id(g_JsonConfig->m_ServerConf["NodeId"].asInt());
	mServerInfo.set_server_name(g_JsonConfig->m_ServerConf["NodeName"].asString());
	mServerInfo.set_server_cur_count(0);
	mServerInfo.set_server_ip(g_JsonConfig->m_ServerConf["NodeIp"].asString());
	mServerInfo.set_server_port(g_JsonConfig->m_ServerConf["NodePort"].asInt());
	mServerInfo.set_server_max_online(2000);
	mServerInfo.set_server_state(EServerState::EST_NORMAL);
	mServerInfo.set_server_type(EServerType::SERVER_TYPE_GAME);
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