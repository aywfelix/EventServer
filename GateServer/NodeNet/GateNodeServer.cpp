#include <iostream>
#include "GateNodeServer.h"
#include "JsonConfig.h"
#include "ClientModule/ModuleChat.h"
#include "ClientPlayer/ClientPlayerMgr.h"
#include "ClientPlayer/ClientPlayer.h"
#include "LogHelper.h"
#include "SeFNodeNet.pb.h"
#include "SeFNet.h"
#include "GateServerThread.h"
#include "SeFNet.h"

using namespace SeFNetProto;

bool GateNodeServer::InitHelper()
{
	mpNetModule->AddReceiveCallBack(this, &GateNodeServer::OtherMessage);
	mpNetModule->AddReceiveCallBack(LOGIN_ROUTE_TO_GATE, this, &GateNodeServer::OnLoginRouteBack);
	mpNetModule->AddReceiveCallBack(CHAT_ROUTE_TO_GATE, this, &GateNodeServer::OnChatRouteBack);

	//init server info
	if (!mpNetModule->InitNet(g_pJsonConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init GateNodeServer failed");
		return false;
	}
	CLOG_INFO << "init GateNodeServer ok" << CLOG_END;
	return true;
}

void GateNodeServer::OtherMessage(const socket_t nFd, const int msgid, const char* msg, const uint32_t nLen)
{
	CLOG_DEBUG << "Gate Server recv other message msgid:" << msgid;
}

bool GateNodeServer::SendPackToLogin(const socket_t client_fd, const int msgid, const char* msg, uint32_t nLen, int nGameID, const std::string& ip)
{
	return true;
}
bool GateNodeServer::SendPackToScene(const int msgid, google::protobuf::Message& xData, int nServerID)
{
	return true;
}
bool GateNodeServer::SentPackToChat(const int msgid, google::protobuf::Message& xData)
{
	ServerDataPtr pServerData = nullptr;
	std::vector<ServerDataPtr> typed_list;
	// 	std::map<int, ServerDataPtr> mmClientNodes;
	for (auto& it : mmClientNodes)
	{
		pServerData = it.second;
		if (pServerData->ServerInfo->server_type() == EServerType::SERVER_TYPE_CHAT)
		{
			typed_list.emplace_back(pServerData);
		}
	}
	if (typed_list.size() == 0)
	{
		CLOG_ERR << "GateNodeServer::SentPackToChat Send ERROR!!!!";
		return false;
	}
	mpNetModule->SendPBMsg(typed_list[0]->fd, msgid, &xData);
	return true;
}
bool GateNodeServer::SentPackToWorld(const int msgid, google::protobuf::Message& xData)
{
	return true;
}

void GateNodeServer::OnLoginRouteBack(socket_t nFd, const int msgid, const char* msg, const uint32_t nLen)
{
}

void GateNodeServer::OnChatRouteBack(socket_t nFd, const int msgid, const char* msg, const uint32_t nLen)
{
	ChatToGatePacket xData;
	if (!mpNetModule->ReceivePB(msgid, msg, nLen, &xData))
	{
		return;
	}

	uint64_t playerId = xData.player_id();

	if (ModuleChat::RPC_CHAT_CHAT_REQ == xData.msg_id())
	{
		ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayerByID(playerId);
		if (pPlayer == nullptr)
		{
			return;
		}

		socket_t nPlayerSock = pPlayer->GetSockFd();
		if (nPlayerSock == -1)
		{
			return;
		}
		g_pServerThread->PlayerServer().SentToClient(xData.msg_id(), xData.msg_body(), nPlayerSock);
	}

	CLOG_INFO << "OnChatRouteBack and sent to client: socket_id:" << playerId << " " << xData.msg_id();

}
void GateNodeServer::OnWorldRouteBack(socket_t nFd, const int msgid, const char *msg, const uint32_t nLen)
{
}