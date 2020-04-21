#include "ClientPlayer.h"
#include "NodeNet/GateServerThread.h"
#include "SeFNodeNet.pb.h"
#include "Session.h"
#include "Socket.h"
#include "LogUtil.h"


bool ClientPlayer::Init()
{
	return true;
}

void ClientPlayer::Clear()
{

}

socket_t ClientPlayer::GetSockFd() 
{ 
	return m_session->GetSocket()->GetFd(); 
}

bool ClientPlayer::SendToGate(const int msg_id, const char* msg, size_t msg_len, socket_t sock_fd)
{
	CLOG_INFO << "gate send to gate msg: " << m_playerid << " msg_id:" << msg_id << CLOG_END;
	return true;
}
bool ClientPlayer::SendToLogin(const int msg_id, const char* msg, size_t msg_len)
{
	GateToLoginPacket gatetologin;
	gatetologin.set_player_id(m_playerid);
	gatetologin.set_msg_id(msg_id);
	gatetologin.set_msg_body(msg, msg_len);
	g_pServerThread->NodeServer().SendToLogin(GATE_ROUTE_TO_LOGIN, &gatetologin);
	CLOG_INFO << "gate send to login msg: " << m_playerid << " msg_id:" << msg_id << CLOG_END;
	return true;
}
bool ClientPlayer::SendToGame(const int msg_id, const char* msg, size_t msg_len)
{
	GateToGamePacket gatetogame;
	gatetogame.set_player_id(m_playerid);
	gatetogame.set_msg_id(msg_id);
	gatetogame.set_msg_body(msg, msg_len);
	g_pServerThread->NodeServer().BroadcastToGame(GATE_ROUTE_TO_GAME, &gatetogame);
	CLOG_INFO << "gate send to game msg: " << m_playerid << " msg_id:" << msg_id << CLOG_END;
	return true;
}
bool ClientPlayer::SendToChat(const int msg_id, const char* msg, size_t msg_len)
{
	GateToChatPacket gatetochat;
	gatetochat.set_player_id(m_playerid);
	gatetochat.set_msg_id(msg_id);
	gatetochat.set_msg_body(msg, msg_len);
	g_pServerThread->NodeServer().SendToChat(GATE_ROUTE_TO_CHAT, &gatetochat);
	CLOG_INFO << "gate send to chat msg:  " << m_playerid << " msg_id:" << msg_id << CLOG_END;
	return true;
}
bool ClientPlayer::SendToWorld(const int msg_id, const char* msg, size_t msg_len)
{
	GateToWorldPacket gatetoworld;
	gatetoworld.set_player_id(m_playerid);
	gatetoworld.set_msg_id(msg_id);
	gatetoworld.set_msg_body(msg, msg_len);
	g_pServerThread->NodeServer().SendToWorld(GATE_ROUTE_TO_WORLD, &gatetoworld);
	CLOG_INFO << "gate send to world msg: " << m_playerid << " msg_id:" << msg_id << CLOG_END;
	return true;
}

void ClientPlayer::SendToClient(const int msg_id, const std::string& msg)
{
	g_pServerThread->PlayerServer().SendToClient(GetSockFd(), msg_id, msg);
	CLOG_INFO << "gate send to player msg: " << m_playerid << " msg_id:" << msg_id << CLOG_END;
}