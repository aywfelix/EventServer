#include "ClientPlayer.h"
#include "NodeNet/GateServerThread.h"
#include "SeFNodeNet.pb.h"
#include "Session.h"
#include "Socket.h"
#include "LogHelper.h"

using namespace SeFNetProto;

bool ClientPlayer::Init()
{
	return true;
}

bool ClientPlayer::Clear()
{
	return true;
}

int ClientPlayer::GetId()
{
	return this->GetMemId();
}

socket_t ClientPlayer::GetSockFd() 
{ 
	return mSession->GetSocket()->GetFd(); 
}

bool ClientPlayer::OnModuleGateMessage(const int msgid, const char* msg, uint32_t nLen, socket_t nFd)
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
	gatetochat.set_player_id(mPlayerId); // TODO Ä¬ÈÏÍæ¼ÒidÎª1
	gatetochat.set_msg_id(msgid);
	gatetochat.set_msg_body(msg, nLen);
	g_pServerThread->NodeServer().SentPackToChat(GATE_ROUTE_TO_CHAT, gatetochat);
	CLOG_INFO << "OnModuleChatMessage " << mPlayerId << "  msgid:" << msgid;
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