#include "LoginNodeClient.h"
#include "JsonConfig.h"
#include "SeFNetClient.h"
#include "packet/PacketMgr.h"
#include "MsgHandle/LoginPlayer.h"
#include "LogUtil.h"

void LoginNodeClient::InitHelper()
{
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_GATE, GATE_ROUTE_TO_LOGIN, this, &LoginNodeClient::OnGateRouteLogin);
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_WORLD, WORLD_ROUTE_TO_LOGIN, this, &LoginNodeClient::OnWorldRouteLogin);
	SetReportInfo();
	AddConnectServer();
}

void LoginNodeClient::SetReportInfo()
{
	mServerInfo.set_server_id(g_pConfig->m_ServerConf["NodeId"].asInt());
	mServerInfo.set_server_name(g_pConfig->m_ServerConf["NodeName"].asString());
	mServerInfo.set_server_cur_count(0);
	mServerInfo.set_server_ip(g_pConfig->m_ServerConf["NodeIp"].asString());
	mServerInfo.set_server_port(g_pConfig->m_ServerConf["NodePort"].asInt());
	mServerInfo.set_server_max_online(2000);
	mServerInfo.set_server_state(EServerState::EST_NORMAL);
	mServerInfo.set_server_type(ServerType::SERVER_TYPE_LOGIN);
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

	Packet* pRecvPacket = g_pPacketMgr->CreatePakcet(gate_packet.msg_id(), gate_packet.msg_body().c_str(), gate_packet.msg_body().length());
	MsgHandle pHandle = g_pPacketMgr->GetMsgHandle(gate_packet.msg_id());
	if (pHandle == nullptr) return;

	LoginPlayer loginPlayer; // TODO loginPlayer 临时变量在查询多线程里会被释放，改成分配内存方式处理
	loginPlayer.m_playerid = gate_packet.player_id();
	loginPlayer.m_servid = pServerData->serv_id;
	if (pHandle(&loginPlayer, pRecvPacket) != 0)
	{
		CLOG_INFO << "OnGateRouteLogin: msg handle error" << CLOG_END;
	}
}

void LoginNodeClient::OnWorldRouteLogin(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	GateToWorldPacket gate_packet;
	if (!ReceivePB(msg_id, msg, msg_len, &gate_packet)) return;

	ConnectDataPtr pServerData = GetServerNetInfo(sock_fd);
	if (!pServerData) return;

	Packet* pRecvPacket = g_pPacketMgr->CreatePakcet(gate_packet.msg_id(), gate_packet.msg_body().c_str(), gate_packet.msg_body().length());
	MsgHandle pHandle = g_pPacketMgr->GetMsgHandle(gate_packet.msg_id());
	if (pHandle == nullptr) return;

	LoginPlayer loginPlayer;
	loginPlayer.m_playerid = gate_packet.player_id();
	loginPlayer.m_servid = pServerData->serv_id;
	if (pHandle(&loginPlayer, pRecvPacket) != 0)
	{
		CLOG_INFO << "OnGateRouteWorld: msg handle error" << CLOG_END;
	}
}

void LoginNodeClient::SendToGate(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pb_msg)
{
	LoginToGatePacket login_packet;
	std::string send_msg = pb_msg->SerializeAsString();
	login_packet.set_msg_id(msg_id);
	login_packet.set_msg_body(send_msg);
	login_packet.set_player_id(playerId);
	mNetCliModule->SendPbByServId(serverid, CHAT_ROUTE_TO_GATE, &login_packet);
}

void LoginNodeClient::SendToWorld(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pb_msg)
{
	LoginToWorldPacket login_packet;
	std::string send_msg = pb_msg->SerializeAsString();
	login_packet.set_msg_id(msg_id);
	login_packet.set_msg_body(send_msg);
	login_packet.set_player_id(playerId);
	mNetCliModule->SendPbByServId(serverid, LOGIN_ROUTE_TO_WORLD, &login_packet);
}
