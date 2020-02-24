#include "LoginPlayer.h"
#include "NodeNet/LoginServerThread.h"

int LoginPlayer::SendToGate(unsigned short msg_id, ::google::protobuf::Message* pb_msg)
{
	g_pServerThread->LoginClient().SendToGate(m_servid, m_playerid, msg_id, pb_msg);
	return 0;
}

int LoginPlayer::SendToWorld(unsigned short msg_id, ::google::protobuf::Message* pb_msg)
{
	g_pServerThread->LoginClient().SendToWorld(m_servid, m_playerid, msg_id, pb_msg);
	return 0;
}
