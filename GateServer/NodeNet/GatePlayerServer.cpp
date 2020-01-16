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


void GatePlayerServer::OnSocketClientEvent(const socket_t sock_fd, const SE_NET_EVENT eEvent, SeNet* pNet)
{
	switch (eEvent)
	{
	case SE_NET_EVENT_EOF:
	case SE_NET_EVENT_ERROR:
	case SE_NET_EVENT_TIMEOUT:
		OnClientDisconnect(sock_fd);
		break;
	case SE_NET_EVENT_CONNECTED:
		OnClientConnected(sock_fd);
		break;
	default:
		break;
	}
}

void GatePlayerServer::OnClientConnected(const socket_t sock_fd)
{
	// bind client'id with socket id
	Session* pSession = m_pNetModule->GetNet()->GetSession(sock_fd);
	ClientPlayer* pPlayer = g_pClientPlayerMgr->NewPlayer(pSession);
	if (pSession == nullptr || pPlayer == nullptr)
	{
		CLOG_ERR << "GatePlayerServer: create player error" << CLOG_END;
		return;
	}
	g_pClientPlayerMgr->AddPlayerIDMap(pPlayer->GetMemId(), pPlayer);
	g_pClientPlayerMgr->AddPlayerSockMap(sock_fd, pPlayer);
	CLOG_INFO << "GatePlayerServer: create player ok Sockfd:" << sock_fd << CLOG_END;
} 

void GatePlayerServer::OnClientDisconnect(const socket_t sock_fd)
{
	ClientPlayer* player = g_pClientPlayerMgr->GetPlayerByFd(sock_fd);
	if (player == nullptr) return;
	g_pClientPlayerMgr->DelPlayer(player);
	CLOG_INFO << "GatePlayerServer: playerid:" << player->GetPlayerId() <<" off line"<<CLOG_END;
}

int GatePlayerServer::GetModuleID(const int msg_id)
{
	return int(msg_id / 100);
}

void GatePlayerServer::OnOtherMessage(const socket_t sock_fd, const int msg_id, const char* msg, const uint32_t msg_len)
{
	if (msg_id <= 0) return;
	int moduleId = GetModuleID(msg_id);

	ClientPlayer* cli_player = g_pClientPlayerMgr->GetPlayerByFd(sock_fd);
	if (cli_player == nullptr) return;

	switch (moduleId)
	{
	case ModuleGate::MODULE_ID_GATE:
	{
		cli_player->OnModuleGateMessage(msg_id, msg, msg_len, sock_fd);
		break;
	}
	case ModuleLogin::MODULE_ID_LOGIN:
	{
		cli_player->OnModuleLoginMessage(msg_id, msg, msg_len);
		break;
	}
	case ModuleChat::MODULE_ID_CHAT:
	{
		cli_player->OnModuleChatMessage(msg_id, msg, msg_len);
		break;
	}
	case ModuleWorld::MODULE_ID_WORLD:
	{
		cli_player->OnModuleWorldMessage(msg_id, msg, msg_len);
		break;
	}
	default:
		cli_player->OnModuleGameMessage(msg_id, msg, msg_len);
		break;
	}
}

void GatePlayerServer::SentToClient(const int msg_id, const std::string& msg, const socket_t sock_fd)
{
	m_pNetModule->SendMsg(sock_fd, msg_id, msg.c_str(), msg.length());
}

void GatePlayerServer::SentToClient(const int msg_id, google::protobuf::Message* msg, const socket_t sock_fd)
{
	m_pNetModule->SendPbMsg(sock_fd, msg_id, msg);
}

void GatePlayerServer::SentToAllClient(const int msg_id, const std::string& msg)
{
	m_pNetModule->SendToAll(msg_id, msg.c_str(), msg.length());
}

void GatePlayerServer::SetClientServerNode(socket_t client_fd, ServerDataPtr& ptr)
{
	ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayer(client_fd);
}

void GatePlayerServer::KickPlayer(const socket_t sock_fd)
{
	m_pNetModule->GetNet()->CloseClient(sock_fd);
}

void GatePlayerServer::KickPlayerAllPlayer()
{
	m_pNetModule->GetNet()->CloseAllClient();
}