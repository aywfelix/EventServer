#include "ChatNodeClient.h"
#include "JsonConfig.h"
#include "packet/PacketMgr.h"
#include "MsgHandle/ChatPlayer.h"
#include "SeFNetClient.h"
#include "SeNet.h"

void ChatNodeClient::InitHelper()
{
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_GATE, GATE_ROUTE_TO_CHAT, this, &ChatNodeClient::OnGateRouteChat);
	mNetCliModule->AddReceiveCallBack(ServerType::SERVER_TYPE_GAME, GAME_ROUTE_TO_CHAT, this, &ChatNodeClient::OnGameRouteChat);
	SetReportInfo();
	AddConnectServer();
}

void ChatNodeClient::SetReportInfo()
{
	mServerInfo.set_server_id(g_JsonConfig->m_ServerConf["NodeId"].asInt());
	mServerInfo.set_server_name(g_JsonConfig->m_ServerConf["NodeName"].asString());
	mServerInfo.set_server_ip(g_JsonConfig->m_ServerConf["NodeIp"].asString());
	mServerInfo.set_server_port(g_JsonConfig->m_ServerConf["NodePort"].asInt());
	mServerInfo.set_server_max_online(2000);
	mServerInfo.set_server_state(EServerState::EST_NORMAL);
	mServerInfo.set_server_type(ServerType::SERVER_TYPE_CHAT);
}

void ChatNodeClient::AddConnectServer()
{
	AddConnectMaster();
	mConnectType.push_back(ServerType::SERVER_TYPE_GATE);
	mConnectType.push_back(ServerType::SERVER_TYPE_GAME);
}

void ChatNodeClient::OnGateRouteChat(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	GateToChatPacket gate_packet;
	if (!SeNet::ReceivePB(msg_id, msg, msg_len, &gate_packet))
		return;

	ConnectDataPtr pServerData = GetServerNetInfo(sock_fd);
	if (!pServerData)
		return;

	// parse the packet
	Packet* pRecvPacket = g_packetmgr->CreatePakcet(gate_packet.msg_id(), gate_packet.msg_body().c_str(), gate_packet.msg_body().length());

	MsgHandle msgHandle = g_packetmgr->GetMsgHandle(gate_packet.msg_id());
	if (msgHandle == nullptr)
		return;

	ChatPlayer chatPlayer;
	chatPlayer.m_PlayerId = gate_packet.player_id();
	chatPlayer.m_ServerId = pServerData->serv_id;
	int ret = msgHandle(&chatPlayer, pRecvPacket); // process msg logic
}

void ChatNodeClient::OnGameRouteChat(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{

}

void ChatNodeClient::SendToGate(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pMsg)
{
	ChatToGatePacket chat_gate;
	std::string send_msg = pMsg->SerializeAsString();
	chat_gate.set_msg_id(msg_id);
	chat_gate.set_msg_body(send_msg);
	chat_gate.set_player_id(playerId);
	mNetCliModule->SendPbByServId(serverid, CHAT_ROUTE_TO_GATE, &chat_gate);
}
