#include "GameNodeClient.h"
#include "JsonConfig.h"
#include "SeFNetClient.h"


void GameNodeClient::InitHelper()
{
    mNetCliModule->AddEventCallBack(ServerType::SERVER_TYPE_MASTER, this, &GameNodeClient::OnSocketEvent);
	mNetCliModule->AddEventCallBack(ServerType::SERVER_TYPE_WORLD, this, &GameNodeClient::OnSocketEvent);
	mNetCliModule->AddEventCallBack(ServerType::SERVER_TYPE_GATE, this, &GameNodeClient::OnSocketEvent);
	
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_GATE, GATE_ROUTE_TO_GAME, this, &GameNodeClient::OnGateRouteGame);
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_WORLD, WORLD_ROUTE_TO_GAME, this, &GameNodeClient::OnWorldRouteGame);
	SetReportInfo();
	AddConnectServer();
}

void GameNodeClient::SetReportInfo()
{
	mServerInfo.set_server_id(g_pConfig->m_ServerConf["NodeId"].asInt());
	mServerInfo.set_server_name(g_pConfig->m_ServerConf["NodeName"].asString());
	mServerInfo.set_server_cur_count(0);
	mServerInfo.set_server_ip(g_pConfig->m_ServerConf["NodeIp"].asString());
	mServerInfo.set_server_port(g_pConfig->m_ServerConf["NodePort"].asInt());
	mServerInfo.set_server_max_online(2000);
	mServerInfo.set_server_state(EServerState::EST_NORMAL);
	mServerInfo.set_server_type(ServerType::SERVER_TYPE_GAME);
}

void GameNodeClient::AddConnectServer()
{
	AddConnectMaster();
	mConnectType.push_back(ServerType::SERVER_TYPE_WORLD);
	mConnectType.push_back(ServerType::SERVER_TYPE_GATE);
}


void GameNodeClient::OnSocketEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
    OnSocketNodeEvent(sock_fd, nEvent, pNet);
}

void GameNodeClient::OnGateRouteGame(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{

}
void GameNodeClient::OnWorldRouteGame(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{

}
void GameNodeClient::SendToGate(const int& server_id, uint64_t player_id, const int msg_id, ::google::protobuf::Message* pb_msg)
{
	GameToGatePacket game_packet;
	std::string send_msg = pb_msg->SerializeAsString();
	game_packet.set_msg_id(msg_id);
	game_packet.set_msg_body(send_msg);
	game_packet.set_player_id(player_id);
	mNetCliModule->SendPbByServId(server_id, GAME_ROUTE_TO_GATE, &game_packet);
}
void GameNodeClient::SendToWorld(const int& server_id, uint64_t player_id, const int msg_id, ::google::protobuf::Message* pb_msg)
{
	GameToWorldPacket game_packet;
	std::string send_msg = pb_msg->SerializeAsString();
	game_packet.set_msg_id(msg_id);
	game_packet.set_msg_body(send_msg);
	game_packet.set_player_id(player_id);
	mNetCliModule->SendPbByServId(server_id, GAME_ROUTE_TO_WORLD, &game_packet);
}