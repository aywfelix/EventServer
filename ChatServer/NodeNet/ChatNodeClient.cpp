#include "ChatNodeClient.h"
#include "JsonConfig.h"
#include "Packet/PacketMgr.h"
#include "Util.h"
#include "Packet/Player.h"
#include "MsgHandle/ChatPlayer.h"
#include "SeFNetClient.h"
#include "SeNet.h"

void ChatNodeClient::InitHelper()
{
	// send chat server info to other server
	mNetCliModule->AddEventCallBack(ServerType::SERVER_TYPE_MASTER, this, &ChatNodeClient::OnSocketEvent);

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

void ChatNodeClient::OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	OnSocketNodeEvent(nFd, nEvent, pNet);
}

void ChatNodeClient::OnGateRouteChat(const socket_t nFd, const int msgid, const char* msg, const uint32_t msglen)
{
	GateToChatPacket xData;
	if (!SeNet::ReceivePB(msgid, msg, msglen, &xData))
		return;

	ConnectDataPtr pServerData = GetServerNetInfo(nFd);
	if (!pServerData)
		return;

	// parse the packet
	Packet* pRecvPacket = g_packetmgr->CreatePakcet(xData.msg_id(), xData.msg_body().c_str(), xData.msg_body().length());

	MsgHandle msgHandle = g_packetmgr->GetMsgHandle(xData.msg_id());
	if (msgHandle == nullptr)
		return;

	ChatPlayer chatPlayer;
	chatPlayer.m_PlayerId = xData.player_id();
	chatPlayer.m_ServerId = pServerData->ServerId;
	int ret = msgHandle(&chatPlayer, pRecvPacket); // process msg logic
}

void ChatNodeClient::OnGameRouteChat(const socket_t nFd, const int msgid, const char* msg, const uint32_t msglen)
{

}

void ChatNodeClient::SendToGate(const int& serverid, uint64_t playerId, uint32_t msg_id, ::google::protobuf::Message* pMsg)
{
	ChatToGatePacket chat_gate;
	std::string send_msg = pMsg->SerializeAsString();
	chat_gate.set_msg_id(msg_id);
	chat_gate.set_msg_body(send_msg);
	chat_gate.set_player_id(playerId);
	mNetCliModule->SendPbByServId(serverid, CHAT_ROUTE_TO_GATE, &chat_gate);
}
