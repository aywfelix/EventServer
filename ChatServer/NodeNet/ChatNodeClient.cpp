#include "ChatNodeClient.h"
#include "JsonConfig.h"
#include "Packet/PacketMgr.h"
#include "Util.h"
#include "Packet/Player.h"
#include "MsgHandle/ChatPlayer.h"
#include "SeFNetClient.h"
#include "SeFINet.h"

using namespace SeFNetProto;

void ChatNodeClient::InitHelper()
{
    mpNetClientModule->AddEventCallBack(EServerType::SERVER_TYPE_MASTER, this, &ChatNodeClient::OnSocketEvent);
	mpNetClientModule->AddEventCallBack(EServerType::SERVER_TYPE_GATE, this, &ChatNodeClient::OnSocketEvent);
	mpNetClientModule->AddReceiveCallBack(EServerType::SERVER_TYPE_GATE, GATE_ROUTE_TO_CHAT, this, &ChatNodeClient::OnGateRouteChat);
	SetServerInfoReport();
	AddConnectServer();
}

void ChatNodeClient::SetServerInfoReport()
{
	mServerReport.set_server_id(g_pJsonConfig->m_ServerConf["NodeId"].asInt());
	mServerReport.set_server_name(g_pJsonConfig->m_ServerConf["NodeName"].asString());
	mServerReport.set_server_cur_count(0);
	mServerReport.set_server_ip(g_pJsonConfig->m_ServerConf["NodeIp"].asString());
	mServerReport.set_server_port(g_pJsonConfig->m_ServerConf["NodePort"].asInt());
	mServerReport.set_server_max_online(2000);
	mServerReport.set_server_state(EServerState::EST_NORMAL);
	mServerReport.set_server_type(SERVER_TYPE_CHAT);
}

void ChatNodeClient::AddConnectServer()
{
	AddConnectMaster();
	mConnectType.emplace_back(SERVER_TYPE_GATE);
}


void ChatNodeClient::OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	OnSocketNodeEvent(nFd, nEvent, pNet);
}

void ChatNodeClient::OnGateRouteChat(const socket_t nFd, const int msgid, const char* msg, const uint32_t msglen)
{
	GateToChatPacket xData;
	if (!SeNet::ReceivePB(msgid, msg, msglen, &xData))
	{
		return;
	}
	ConnectDataPtr pServerData = GetServerNetInfo(nFd);
	if (!pServerData)
	{
		return; //TODO pServerData allways null
	}
	//parse the packet
	Packet* pRecvPacket = g_pPacketMgr->CreatePakcet(xData.msg_id(), xData.msg_body().c_str(), xData.msg_body().length());
	AutoFree<Packet> freeRecvPacket(pRecvPacket);

	MsgHandle msgHandle = g_pPacketMgr->GetMsgHandle(xData.msg_id());
	if (msgHandle == nullptr)
	{
		return;
	}
	ChatPlayer* pPlayer = new ChatPlayer;
	AutoFree<Player> freePlayer(pPlayer);
	pPlayer->m_PlayerId = xData.player_id();
	pPlayer->m_ServerId = pServerData->ServerId;
	int ret = msgHandle(pPlayer, pRecvPacket);  //process msg logic

}

void ChatNodeClient::SendToGate(const int GameID, uint64_t playerId, uint32_t msg_id, ::google::protobuf::Message* pMsg)
{
	std::string send_msg;

	ChatToGatePacket chattogate;
	pMsg->SerializeToString(&send_msg);
	chattogate.Clear();
	chattogate.set_msg_id(msg_id);
	chattogate.set_msg_body(send_msg);
	chattogate.set_player_id(playerId);
	mpNetClientModule->SendPBByServerId(GameID, CHAT_ROUTE_TO_GATE, &chattogate);
}