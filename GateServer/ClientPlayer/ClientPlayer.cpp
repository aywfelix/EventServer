#include "ClientPlayer.h"
#include "NodeNet/GateServerThread.h"
#include "SeFNodeNet.pb.h"
#include "Session.h"
#include "Socket.h"
#include "LogHelper.h"



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

bool ClientPlayer::OnModuleGateMessage(const int msg_id, const char* msg, uint32_t msg_len, socket_t sock_fd)
{
	return true;
}
bool ClientPlayer::OnModuleLoginMessage(const int msg_id, const char* msg, uint32_t msg_len)
{
	return true;
}
bool ClientPlayer::OnModuleGameMessage(const int msg_id, const char* msg, uint32_t msg_len)
{
	return true;
}
bool ClientPlayer::OnModuleChatMessage(const int msg_id, const char* msg, uint32_t msg_len)
{
	GateToChatPacket gatetochat;
	gatetochat.set_player_id(mPlayerId);
	gatetochat.set_msg_id(msg_id);
	gatetochat.set_msg_body(msg, msg_len);
	g_pServerThread->NodeServer().SentPackToChat(GATE_ROUTE_TO_CHAT, &gatetochat);
	CLOG_INFO << "OnModuleChatMessage " << mPlayerId << "  msg_id:" << msg_id;
	return true;
}
bool ClientPlayer::OnModuleWorldMessage(const int msg_id, const char* msg, uint32_t msg_len)
{
	return true;
}

void ClientPlayer::SendToClient(const int msg_id, const std::string& msg)
{
	g_pServerThread->PlayerServer().SentToClient(msg_id, msg, GetSockFd());
}