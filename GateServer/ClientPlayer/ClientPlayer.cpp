#include "ClientPlayer/ClientPlayer.h"
#include "NodeNet/GateServerThread.h"
#include "ServerProto/NFNodeNet.pb.h"
#include "easylogging/easylogging++.h"

ClientPlayer::ClientPlayer(int id):m_id(id)
{

}
ClientPlayer::ClientPlayer()
{

}
ClientPlayer::~ClientPlayer()
{

}

void ClientPlayer::Clear()
{

}

NFSOCK ClientPlayer::GetSockFd()
{
	return m_pNetObject->GetRealFD();
}

bool ClientPlayer::OnModuleGateMessage(const int msgid, const char* msg, uint32_t nLen, NFSOCK nSockIndex)
{
	return true;
}
bool ClientPlayer::OnModuleLoginMessage(const int msgid, const char* msg, uint32_t nLen)
{
	return true;
}
bool ClientPlayer::OnModuleGameMessage(const int msgid, const char* msg, uint32_t nLen)
{
	return true;
}
bool ClientPlayer::OnModuleChatMessage(const int msgid, const char* msg, uint32_t nLen)
{
	GateToChatPacket gatetochat;
	gatetochat.set_player_id(m_PlayerId); //TODO Ä¬ÈÏÍæ¼ÒidÎª1
	gatetochat.set_msg_id(msgid);
	gatetochat.set_msg_body(msg, nLen);
	g_pServerThread->NodeServer().SentPackToChat(GATE_ROUTE_TO_CHAT, gatetochat);
	LOG(INFO) << "OnModuleChatMessage " << m_PlayerId << "  msgid:" << msgid;
	return true;
}
bool ClientPlayer::OnModuleWorldMessage(const int msgid, const char* msg, uint32_t nLen)
{

	return true;
}

void ClientPlayer::SendToClient(const int msgid, const std::string& msg)
{
	g_pServerThread->PlayerServer().SentToClient(msgid, msg, GetSockFd());
}