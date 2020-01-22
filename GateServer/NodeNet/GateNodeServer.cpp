#include "GateNodeServer.h"
#include "GateServerThread.h"
#include "JsonConfig.h"
#include "clientmodule/ModuleChat.h"
#include "ClientPlayer/ClientPlayerMgr.h"
#include "LogUtil.h"
#include "SeFNodeNet.pb.h"
#include "SeFNet.h"


bool GateNodeServer::InitHelper()
{
	mNetServModule->AddReceiveCallBack(this, &GateNodeServer::OtherMessage);
	mNetServModule->AddReceiveCallBack(LOGIN_ROUTE_TO_GATE, this, &GateNodeServer::OnLoginRouteBack);
	mNetServModule->AddReceiveCallBack(CHAT_ROUTE_TO_GATE, this, &GateNodeServer::OnChatRouteBack);

	//init server info
	if (!mNetServModule->InitNet(g_JsonConfig->m_ServerConf["NodePort"].asUInt()))
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

bool GateNodeServer::SendPackToLogin(const int msg_id, google::protobuf::Message* pb_msg)
{
	SendPbByServTypeOne(SERVER_TYPE_LOGIN, msg_id, pb_msg);
	return true;
}
bool GateNodeServer::SendPackToScene(const int msg_id, google::protobuf::Message* pb_msg, int server_id)
{
	return true;
}
bool GateNodeServer::SentPackToChat(const int msg_id, google::protobuf::Message* pb_msg)
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
bool GateNodeServer::SentPackToWorld(const int msg_id, google::protobuf::Message* msg)
{
	return true;
}

void GateNodeServer::OnLoginRouteBack(socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
}

void GateNodeServer::OnChatRouteBack(socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	ChatToGatePacket chat_packet;
	if (!ReceivePB(msg_id, msg, msg_len, &chat_packet)) return;

	uint64_t playerId = chat_packet.player_id();
	if (ModuleChat::RPC_CHAT_CHAT_REQ == chat_packet.msg_id())
	{
		ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayerByID(playerId);
		if (pPlayer == nullptr) return;

		socket_t nPlayerSock = pPlayer->GetSockFd();
		if (nPlayerSock == -1) return;

		g_pServerThread->PlayerServer().SentToClient(chat_packet.msg_id(), chat_packet.msg_body(), nPlayerSock);
	}

	CLOG_INFO << "OnChatRouteBack and sent to client: socket_id:" << playerId << " " << chat_packet.msg_id();

}
void GateNodeServer::OnWorldRouteBack(socket_t sock_fd, const int msg_id, const char *msg, const size_t msg_len)
{
}