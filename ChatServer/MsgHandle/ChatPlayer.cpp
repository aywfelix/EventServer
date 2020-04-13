#include "MsgHandle/ChatPlayer.h"
#include "NodeNet/ChatServerThread.h"

int ChatPlayer::SendToGate(const int msg_id, ::google::protobuf::Message* pb_msg)
{
	g_pServerThread->ChatClient().SendToGate(m_servid, m_playerid, msg_id, pb_msg);
	return 0;
}

int ChatPlayer::SendToGame(const int msg_id, ::google::protobuf::Message* pb_msg)
{
	g_pServerThread->ChatServer().SendToGame(msg_id, pb_msg);
	return 0;
}