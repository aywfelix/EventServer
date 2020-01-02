#include <iostream>
#include "GateNodeServer.h"
#include "JsonConfig.h"
#include "ClientModule/ModuleChat.h"
#include "ClientPlayer/ClientPlayerMgr.h"
#include "ClientPlayer/ClientPlayer.h"
#include "NodeNet/GateServerThread.h"
#include "LogHelper.h"

bool GateNodeServer::InitHelper()
{
	m_pNetModule->AddReceiveCallBack(this, &GateNodeServer::OtherMessage);
	m_pNetModule->AddReceiveCallBack(LOGIN_ROUTE_TO_GATE, this, &GateNodeServer::OnLoginRouteBack);
	m_pNetModule->AddReceiveCallBack(CHAT_ROUTE_TO_GATE, this, &GateNodeServer::OnChatRouteBack);

	//init server info
	if (!mpNetModule->InitNet(g_pJsonConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init MasterNodeServer failed");
		return false;
	}
	return true;
	return true;
}

void GateNodeServer::OtherMessage(const socket_t nSockIndex, const int msgid, const char *msg, const uint32_t nLen)
{
	LOG(INFO) << "Gate Server recv other message msgid:" << msgid;
}

bool GateNodeServer::SendPackToLogin(const socket_t client_fd, const int msgid, const char *msg, uint32_t nLen, int nGameID, const std::string &ip)
{
	return true;
}
bool GateNodeServer::SendPackToScene(const int msgid, const google::protobuf::Message &xData, int nServerID)
{
	return true;
}
bool GateNodeServer::SentPackToChat(const int msgid, const google::protobuf::Message &xData)
{
	ServerDataPtr pServerData = m_mClientNodes.First();
	std::vector<ServerDataPtr> typed_list;
	while (pServerData)
	{
		if (EServerType::SERVER_TYPE_CHAT == pServerData->pData->server_type())
		{
			typed_list.push_back(pServerData);
		}
		pServerData = m_mClientNodes.Next();
	}
	if (typed_list.size() == 0)
	{
		LOG(ERROR) << "GateNodeServer::SentPackToChat Send ERROR!!!!";
		return false;
	}
	return m_pNetModule->SendMsgPB(msgid, xData, typed_list[0]->nFD);
}
bool GateNodeServer::SentPackToWorld(const int msgid, const google::protobuf::Message &xData)
{
	return true;
}

void GateNodeServer::OnLoginRouteBack(socket_t nSockIndex, const int msgid, const char *msg, const uint32_t nLen)
{
}

void GateNodeServer::OnChatRouteBack(socket_t nSockIndex, const int msgid, const char *msg, const uint32_t nLen)
{
	NFGUID nGUID;
	ChatToGatePacket xData;
	if (!m_pNetModule->ReceivePB(msgid, msg, nLen, xData, nGUID))
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

	LOG(INFO) << "OnChatRouteBack and sent to client: socket_id:" << playerId << " " << xData.msg_id();

}
void GateNodeServer::OnWorldRouteBack(socket_t nSockIndex, const int msgid, const char *msg, const uint32_t nLen)
{
}