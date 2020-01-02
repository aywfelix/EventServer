
#include "NodeNet/GatePlayerServer.h"
#include "ClientPlayer/ClientPlayer.h"
#include "ClientPlayer/ClientPlayerMgr.h"
#include "ServerComm/Config.h"
#include "ClientModule/ModuleChat.h"
#include "ClientModule/ModuleGate.h"
#include "ClientModule/ModuleLogin.h"
#include "ClientModule/ModuleWorld.h"
#include "easylogging/easylogging++.h"


bool GatePlayerServer::Init()
{
	m_pNetModule = new NFNetModule();
	m_pNetModule->ExpandBufferSize(1024 * 1024 * 20);  //20k cache
	m_pNetModule->NFINetModule::AddEventCallBack(this, &GatePlayerServer::OnSocketClientEvent);
	m_pNetModule->NFINetModule::AddReceiveCallBack(this, &GatePlayerServer::OnOtherMessage);

	m_pNetModule->NFINetModule::AddReceiveCallBack(ModuleChat::RPC_CHAT_CHAT_REQ, this, &GatePlayerServer::OnOtherMessage);
	int ret = m_pNetModule->Initialization(
		g_pConfig->m_Root["GatePlayerServer"]["NodeIp"].asCString(),
		g_pConfig->m_Root["GatePlayerServer"]["NodePort"].asInt(),
		g_pConfig->m_Root["GatePlayerServer"]["MaxConnect"].asInt()
	);
	if (ret < 0)
	{
		LOG(ERROR) << "init GatePlayerServer failed";
		return false;
	}
	return true;
}

bool GatePlayerServer::Loop()
{
	return m_pNetModule->Execute();
}


void GatePlayerServer::OnSocketClientEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	switch (eEvent)
	{
	case NF_NET_EVENT_EOF:
	case NF_NET_EVENT_ERROR:
	case NF_NET_EVENT_TIMEOUT:
		OnClientDisconnect(nSockIndex);
		break;
	case NF_NET_EVENT_CONNECTED:
		OnClientConnected(nSockIndex);
		break;
	default:
		
		break;
	}
}

void GatePlayerServer::OnClientConnected(const NFSOCK nSockIndex)
{
	//bind client'id with socket id

	NetObject* pNetObject = m_pNetModule->GetNet()->GetNetObject(nSockIndex);
	if (pNetObject)
	{
		ClientPlayer* pClientPlayer = g_pClientPlayerMgr->CreatePlayer(pNetObject);
		g_pClientPlayerMgr->AddPlayerIDMap(1, pClientPlayer); //TODO 测试 默认playerid 为 1
		LOG(INFO) << "gate player server create player ok Sockfd:"<< nSockIndex;
	}
	else
	{
		LOG(ERROR) << "gate player server create player error netobject is nullptr";
	}
}

void GatePlayerServer::OnClientDisconnect(const NFSOCK nSockIndex)
{
	ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayer(nSockIndex);
	if (pPlayer == nullptr)
	{
		return;
	}
	else
	{
		g_pClientPlayerMgr->DestoryPlayer(pPlayer);
	}
}

int GatePlayerServer::GetModuleID(const int msgid)
{
	return int(msgid / 100);
}

void GatePlayerServer::OnOtherMessage(const NFSOCK nSockIndex, const int msgid, const char* msg, const uint32_t nLen)
{
	if (msgid <= 0)
	{
		return;
	}

	int moduleId = GetModuleID(msgid);

	ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayer(nSockIndex);
	if (pPlayer == nullptr)
	{
		return;
	}
	switch (moduleId)
	{
	case ModuleGate::MODULE_ID_GATE:
	{
		pPlayer->OnModuleGateMessage(msgid, msg, nLen, nSockIndex);
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



void GatePlayerServer::SentToClient(const int nMsgID, const std::string& msg, const NFSOCK nSockIndex)
{
	m_pNetModule->SendMsgWithOutHead(nMsgID, msg, nSockIndex);
}

void GatePlayerServer::SentToClient(int nMsgID, const google::protobuf::Message& xData, NFSOCK nSockIndex)
{
	m_pNetModule->SendMsgPB(nMsgID, xData, nSockIndex);
}

void GatePlayerServer::SentToAllClient(const int nMsgID, const std::string& msg)
{
	m_pNetModule->SendMsgToAllClientWithOutHead(nMsgID, msg);
}

void GatePlayerServer::SetClientServerNode(NFSOCK client_fd, NF_SHARE_PTR<ServerData> spServerData)
{
	ClientPlayer* pPlayer = g_pClientPlayerMgr->GetPlayer(client_fd);
	if (pPlayer != nullptr && spServerData != nullptr)
	{
		//pPlayer->AttachServerData(spServerData);
		return;

	}
	else
	{
		return;
	}

}

void GatePlayerServer::KickPlayer(const NFSOCK nSockIndex)
{
	m_pNetModule->GetNet()->CloseNetObject(nSockIndex);
}

void GatePlayerServer::KickPlayerAllPlayer()
{
	m_pNetModule->GetNet()->KickAll();
}