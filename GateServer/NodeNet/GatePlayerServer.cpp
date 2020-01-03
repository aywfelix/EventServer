
#include "NodeNet/GatePlayerServer.h"
#include "ClientPlayer/ClientPlayer.h"
#include "ClientPlayer/ClientPlayerMgr.h"
#include "JsonConfig.h"
#include "ClientModule/ModuleChat.h"
#include "ClientModule/ModuleGate.h"
#include "ClientModule/ModuleLogin.h"
#include "ClientModule/ModuleWorld.h"
#include "LogHelper.h"
#include "SeFINet.h"
#include "LogHelper.h"


bool GatePlayerServer::Init()
{
	m_pNetModule = new SeFNet();
	m_pNetModule->AddEventCallBack(this, &GatePlayerServer::OnSocketClientEvent);
	m_pNetModule->AddReceiveCallBack(this, &GatePlayerServer::OnOtherMessage);
	m_pNetModule->AddReceiveCallBack(ModuleChat::RPC_CHAT_CHAT_REQ, this, &GatePlayerServer::OnOtherMessage);
	//init server info
	if (!m_pNetModule->InitNet(g_pJsonConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init MasterNodeServer failed");
		return false;
	}
	return true;
}

void GatePlayerServer::Loop()
{
	m_pNetModule->Excute(LOOP_RUN_NONBLOCK);
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
	//NetObject* pNetObject = m_pNetModule->GetNet()->GetNetObject(nFd);
	//if (pNetObject)
	//{
	//	ClientPlayer* pClientPlayer = g_pClientPlayerMgr->CreatePlayer(pNetObject);
	//	g_pClientPlayerMgr->AddPlayerIDMap(1, pClientPlayer); // TODO 测试 默认playerid 为 1
	//	CLOG_INFO << "gate player server create player ok Sockfd:" << nFd;

	//}
	//else
	//{
	//	CLOG_ERR << "gate player server create player error netobject is nullptr";
	//}
}

void GatePlayerServer::OnClientDisconnect(const socket_t nFd)
{
	ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayer(nFd);
	if (pPlayer == nullptr)
	{
		return;
	}
	else
	{
		/*g_pClientPlayerMgr->DestoryPlayer(pPlayer);*/
	}
}

int GatePlayerServer::GetModuleID(const int msgid)
{
	return int(msgid / 100);
}

void GatePlayerServer::OnOtherMessage(const socket_t nFd, const int msgid, const char* msg, const uint32_t nLen)
{
	if (msgid <= 0)
	{
		return;
	}

	int moduleId = GetModuleID(msgid);

	ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayer(nFd);
	if (pPlayer == nullptr)
	{
		return;
	}
	switch (moduleId)
	{
	case ModuleGate::MODULE_ID_GATE:
	{
		pPlayer->OnModuleGateMessage(msgid, msg, nLen, nFd);
		break;
	}
	case ModuleLogin::MODULE_ID_LOGIN:
	{
		pPlayer->OnModuleLoginMessage(msgid, msg, nLen);
		break;
	}
	case ModuleChat::MODULE_ID_CHAT:
	{
		pPlayer->OnModuleChatMessage(msgid, msg, nLen);
		break;
	}
	case ModuleWorld::MODULE_ID_WORLD:
	{
		pPlayer->OnModuleWorldMessage(msgid, msg, nLen);
		break;
	}
	default:
		pPlayer->OnModuleGameMessage(msgid, msg, nLen);
		break;
	}

}

void GatePlayerServer::SentToClient(const int nMsgID, const std::string& msg, const socket_t nFd)
{
	m_pNetModule->SendMsg(nFd, nMsgID, msg.c_str(), msg.length());
}

void GatePlayerServer::SentToClient(const int nMsgID, google::protobuf::Message& xData, socket_t nFd)
{
	m_pNetModule->SendPBMsg(nFd, nMsgID, &xData);
}

void GatePlayerServer::SentToAllClient(const int nMsgID, const std::string& msg)
{
	m_pNetModule->SendToAllMsg(nMsgID, msg.c_str(), msg.length());
}

void GatePlayerServer::SetClientServerNode(socket_t client_fd, ServerDataPtr& ptr)
{
	ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayer(client_fd);
	if (pPlayer != nullptr)
	{
		//pPlayer->AttachServerData(spServerData);
		return;

	}
	else
	{
		return;
	}
}

void GatePlayerServer::KickPlayer(const socket_t nFd)
{
	m_pNetModule->GetNet()->CloseClient(nFd);
}

void GatePlayerServer::KickPlayerAllPlayer()
{
	m_pNetModule->GetNet()->CloseAllClient();
}