#include "MsgHandle/ChatPlayer.h"
#include "NodeNet/ChatServerThread.h"

int ChatPlayer::SendToGate(unsigned short msg_id, ::google::protobuf::Message* pb_msg)
{
	g_pServerThread->ChatClient().SendToGate(m_servid, m_playerid, msg_id, pb_msg);
	return 0;
}

int ChatPlayer::SendToGame(unsigned short msg_id, ::google::protobuf::Message* pb_msg)
{
	g_pServerThread->ChatServer().SendPackToGame(msg_id, pb_msg);
	return 0;
}