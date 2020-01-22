#include "LoginNodeClient.h"
#include "JsonConfig.h"
#include "SeFNetClient.h"
#include "packet/PacketMgr.h"
#include "MsgHandle/LoginPlayer.h"
#include "LogUtil.h"

void LoginNodeClient::InitHelper()
{
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_GATE, GATE_ROUTE_TO_CHAT, this, &LoginNodeClient::OnGateRouteLogin);
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_WORLD, GAME_ROUTE_TO_CHAT, this, &LoginNodeClient::OnDBRouteLogin);
	SetReportInfo();
	AddConnectServer();
}

void LoginNodeClient::SetReportInfo()
{
	mServerInfo.set_server_id(g_JsonConfig->m_ServerConf["NodeId"].asInt());
	mServerInfo.set_server_name(g_JsonConfig->m_ServerConf["NodeName"].asString());
	mServerInfo.set_server_cur_count(0);
	mServerInfo.set_server_ip(g_JsonConfig->m_ServerConf["NodeIp"].asString());
	mServerInfo.set_server_port(g_JsonConfig->m_ServerConf["NodePort"].asInt());
	mServerInfo.set_server_max_online(2000);
	mServerInfo.set_server_state(EServerState::EST_NORMAL);
	mServerInfo.set_server_type(ServerType::SERVER_TYPE_GAME);
}

void LoginNodeClient::AddConnectServer()
{
	AddConnectMaster();
	mConnectType.push_back(ServerType::SERVER_TYPE_WORLD);
	mConnectType.push_back(ServerType::SERVER_TYPE_GATE);
}

void LoginNodeClient::OnGateRouteLogin(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	GateToLoginPacket gate_packet;
	if (!ReceivePB(msg_id, msg, msg_len, &gate_packet)) return;

	ConnectDataPtr pServerData = GetServerNetInfo(sock_fd);
	if (!pServerData) return;

	Packet* pRecvPacket = g_packetmgr->CreatePakcet(gate_packet.msg_id(), gate_packet.msg_body().c_str(), gate_packet.msg_body().length());
	MsgHandle pHandle = g_packetmgr->GetMsgHandle(gate_packet.msg_id());
	if (pHandle == nullptr) return;

	LoginPlayer loginPlayer;
	loginPlayer.m_playerid = gate_packet.player_id();
	loginPlayer.m_servid = pServerData->serv_id;
	if (pHandle(&loginPlayer, pRecvPacket) != 0)
	{
		CLOG_INFO << "OnGateRouteLogin: msg handle error" << CLOG_END;
	}
}
void LoginNodeClient::OnDBRouteLogin(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{

}
void LoginNodeClient::SendToGate(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pb_msg)
{

}
void LoginNodeClient::SendToDB(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pb_msg)
{

}
void LoginNodeClient::SendToWorld(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pb_msg)
{

}
