#include "MsgHandle/ChatPlayer.h"
#include "NodeNet/ChatServerThread.h"

int ChatPlayer::SendToGate(unsigned short msg_id, ::google::protobuf::Message* pMsg)
{
	g_pServerThread->ChatClient().SendToGate(m_ServerId, m_PlayerId, msg_id, pMsg);
	return 0;
}

int ChatPlayer::SendToGame(unsigned short msg_id, ::google::protobuf::Message* pMsg)
{
	g_pServerThread->ChatServer().SendPackToGame(msg_id, *pMsg);
	return 0;
}