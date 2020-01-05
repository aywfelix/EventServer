#include "MsgHandle/ChatPlayer.h"
#include "NodeNet/ChatServerThread.h"

int ChatPlayer::SendMsg(unsigned short msg_id, ::google::protobuf::Message* pMsg)
{
	g_pServerThread->ChatClient().SendToGate(m_ServerId, m_PlayerId, msg_id, pMsg);
	return 0;
}