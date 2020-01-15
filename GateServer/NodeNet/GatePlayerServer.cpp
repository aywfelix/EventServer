#include "SeFServerBase.h"
#include "SeFINet.h"
#include "SeFNet.h"

#include "NodeNet/GatePlayerServer.h"
#include "ClientPlayer/ClientPlayerMgr.h"
#include "LogHelper.h"
#include "JsonConfig.h"
#include "Session.h"

#include "ClientModule/ModuleChat.h"
#include "ClientModule/ModuleGate.h"
#include "ClientModule/ModuleLogin.h"
#include "ClientModule/ModuleWorld.h"

bool GatePlayerServer::Init()
{
	m_pNetModule = new SeFNet();
	m_pNetModule->AddEventCallBack(this, &GatePlayerServer::OnSocketClientEvent);
	m_pNetModule->AddReceiveCallBack(this, &GatePlayerServer::OnOtherMessage);
	m_pNetModule->AddReceiveCallBack(ModuleChat::RPC_CHAT_CHAT_REQ, this, &GatePlayerServer::OnOtherMessage);
	//init server info
	Json::Value GatePlayerServerConf = g_JsonConfig->m_Root["GatePlayerServer"];
	if (!m_pNetModule->InitNet(GatePlayerServerConf["NodePort"].asUInt()))
	{
		CLOG_ERR << "init GatePlayerServer failed" << CLOG_END;
		//LOG_ERR("init GatePlayerServer failed");
		return false;
	}
	CLOG_INFO << "init GatePlayerServer ok" << CLOG_END;
	return true;
}

void GatePlayerServer::Loop()
{
	m_pNetModule->Execute(LOOP_RUN_NONBLOCK);
}


void GatePlayerServer::OnSocketClientEvent(const socket_t nFd, const SE_NET_EVENT eEvent, SeNet* pNet)
{
	switch (eEvent)
	{
	case SE_NET_EVENT_EOF:
	case SE_NET_EVENT_ERROR:
	case SE_NET_EVENT_TIMEOUT:
		OnClientDisconnect(nFd);
		break;
	case SE_NET_EVENT_CONNECTED:
		OnClientConnected(nFd);
		break;
	default:
		break;
	}
}

void GatePlayerServer::OnClientConnected(const socket_t nFd)
{
	// bind client'id with socket id
	Session* pSession = m_pNetModule->GetNet()->GetSession(nFd);
	ClientPlayer* pPlayer = g_pClientPlayerMgr->NewPlayer(pSession);
	if (pSession == nullptr || pPlayer == nullptr)
	{
		CLOG_ERR << "GatePlayerServer: create player error" << CLOG_END;
		return;
	}
	g_pClientPlayerMgr->AddPlayerIDMap(pPlayer->GetMemId(), pPlayer);
	g_pClientPlayerMgr->AddPlayerSockMap(nFd, pPlayer);
	CLOG_INFO << "GatePlayerServer: create player ok Sockfd:" << nFd << CLOG_END;
} 

void GatePlayerServer::OnClientDisconnect(const socket_t nFd)
{
	ClientPlayer* player = g_pClientPlayerMgr->GetPlayerByFd(nFd);
	if (player == nullptr) return;
	g_pClientPlayerMgr->DelPlayer(player);
	CLOG_INFO << "GatePlayerServer: playerid:" << player->GetPlayerId() <<" off line"<<CLOG_END;
}

int GatePlayerServer::GetModuleID(const int msgid)
{
	return int(msgid / 100);
}

void GatePlayerServer::OnOtherMessage(const socket_t nFd, const int msgid, const char* msg, const uint32_t nLen)
{
	if (msgid <= 0) return;
	int moduleId = GetModuleID(msgid);

	ClientPlayer* cli_player = g_pClientPlayerMgr->GetPlayerByFd(nFd);
	if (cli_player == nullptr) return;

	switch (moduleId)
	{
	case ModuleGate::MODULE_ID_GATE:
	{
		cli_player->OnModuleGateMessage(msgid, msg, nLen, nFd);
		break;
	}
	case ModuleLogin::MODULE_ID_LOGIN:
	{
		cli_player->OnModuleLoginMessage(msgid, msg, nLen);
		break;
	}
	case ModuleChat::MODULE_ID_CHAT:
	{
		cli_player->OnModuleChatMessage(msgid, msg, nLen);
		break;
	}
	case ModuleWorld::MODULE_ID_WORLD:
	{
		cli_player->OnModuleWorldMessage(msgid, msg, nLen);
		break;
	}
	default:
		cli_player->OnModuleGameMessage(msgid, msg, nLen);
		break;
	}
}

void GatePlayerServer::SentToClient(const int nMsgID, const std::string& msg, const socket_t nFd)
{
	m_pNetModule->SendMsg(nFd, nMsgID, msg.c_str(), msg.length());
}

void GatePlayerServer::SentToClient(const int nMsgID, google::protobuf::Message* xData, const socket_t nFd)
{
	m_pNetModule->SendPbMsg(nFd, nMsgID, &xData);
}

void GatePlayerServer::SentToAllClient(const int nMsgID, const std::string& msg)
{
	m_pNetModule->SendToAll(nMsgID, msg.c_str(), msg.length());
}

void GatePlayerServer::SetClientServerNode(socket_t client_fd, ServerDataPtr& ptr)
{
	ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayer(client_fd);
}

void GatePlayerServer::KickPlayer(const socket_t nFd)
{
	m_pNetModule->GetNet()->CloseClient(nFd);
}

void GatePlayerServer::KickPlayerAllPlayer()
{
	m_pNetModule->GetNet()->CloseAllClient();
}