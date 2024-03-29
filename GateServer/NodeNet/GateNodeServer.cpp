#include "GateNodeServer.h"
#include "GateServerThread.h"
#include "JsonConfig.h"
#include "ClientPlayer/ClientPlayerMgr.h"
#include "LogUtil.h"
#include "SeFNodeNet.pb.h"
#include "SeFNet.h"
#include "client/chat/HandleChat.h"


bool GateNodeServer::InitHelper()
{
	mNetServModule->AddReceiveCallBack(this, &GateNodeServer::OtherMessage);
	mNetServModule->AddReceiveCallBack(LOGIN_ROUTE_TO_GATE, this, &GateNodeServer::OnLoginRouteGate);
	mNetServModule->AddReceiveCallBack(CHAT_ROUTE_TO_GATE, this, &GateNodeServer::OnChatRouteGate);

	//init server info
	if (!mNetServModule->InitNet(g_pConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init GateNodeServer failed");
		return false;
	}
	CLOG_INFO << "init GateNodeServer ok" << CLOG_END;
	return true;
}

void GateNodeServer::OtherMessage(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	CLOG_DEBUG << "Gate Server recv other message msg_id:" << msg_id;
}

bool GateNodeServer::SendToLogin(const int msg_id, google::protobuf::Message* pb_msg)
{
	SendPbByServTypeOne(SERVER_TYPE_LOGIN, msg_id, pb_msg);
	return true;
}
bool GateNodeServer::BroadcastToGame(const int msg_id, google::protobuf::Message* pb_msg)
{
	for (auto& it : mmServNodes)
	{
		ServerDataPtr pServerData = it.second;
		if (pServerData->ServerInfo->server_type() == ServerType::SERVER_TYPE_GAME)
		{
			mNetServModule->SendPbMsg(pServerData->fd, msg_id, pb_msg);
		}
	}
	return true;
}

bool GateNodeServer::SendToGame(const int server_id, const int msg_id, google::protobuf::Message* pb_msg)
{
	SendPbByServId(server_id, msg_id, pb_msg);
	return true;
}
bool GateNodeServer::SendToChat(const int msg_id, google::protobuf::Message* pb_msg)
{
	ServerDataPtr pServerData = nullptr;
	std::vector<ServerDataPtr> cli_list;

	for (auto& it : mmServNodes)
	{
		pServerData = it.second;
		if (pServerData->ServerInfo->server_type() == ServerType::SERVER_TYPE_CHAT)
		{
			cli_list.emplace_back(pServerData);
		}
	}
	if (cli_list.size() == 0)
	{
		CLOG_ERR << "GateNodeServer::SentPackToChat Send ERROR!!!!";
		return false;
	}
	mNetServModule->SendPbMsg(cli_list[0]->fd, msg_id, pb_msg);
	return true;
}
bool GateNodeServer::SendToWorld(const int msg_id, google::protobuf::Message* msg)
{
	return true;
}

void GateNodeServer::OnLoginRouteGate(socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	LoginToGatePacket login_packet;
	if (msg == nullptr || !(login_packet.ParseFromArray(msg, msg_len))) return;

	uint64_t playerId = login_packet.player_id();
	ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayerByID(playerId);
	if (pPlayer == nullptr) return;

	socket_t player_fd = pPlayer->GetSockFd();
	if (player_fd == -1) return;

	g_pServerThread->PlayerServer().SendToClient(player_fd, login_packet.msg_id(), &login_packet);

	CLOG_INFO << "OnLoginRouteGate and sent to client: socket_id:" << playerId << " " << login_packet.msg_id();
}

void GateNodeServer::OnChatRouteGate(socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	ChatToGatePacket chat_packet;
	if (msg == nullptr || !(chat_packet.ParseFromArray(msg, msg_len))) return;

	uint64_t playerId = chat_packet.player_id();
	if (HandleChat::RPC_CHAT_CHAT_REQ == chat_packet.msg_id())
	{
		ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayerByID(playerId);
		if (pPlayer == nullptr) return;

		socket_t player_fd = pPlayer->GetSockFd();
		if (player_fd == -1) return;

		g_pServerThread->PlayerServer().SendToClient(player_fd, chat_packet.msg_id(), &chat_packet);
	}

	CLOG_INFO << "OnChatRouteBack and sent to client: socket_id:" << playerId << " " << chat_packet.msg_id();

}
void GateNodeServer::OnWorldRouteGate(socket_t sock_fd, const int msg_id, const char *msg, const size_t msg_len)
{

}