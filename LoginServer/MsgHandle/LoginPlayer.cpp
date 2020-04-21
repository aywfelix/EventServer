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


std::unique_ptr<LoginPlayerPool> g_pLoginPlayerPool = nullptr;

LoginPlayerPool::LoginPlayerPool() {}
LoginPlayerPool::~LoginPlayerPool() {}

LoginPlayer* LoginPlayerPool::NewLoginPlayer()
{
	LoginPlayer* player = m_player_pool.NewElem();
	if (player == nullptr) return nullptr;
	return player;
}

LoginPlayer* LoginPlayerPool::GetLoginPlayer(int32_t memid)
{
	LoginPlayer* player = m_player_pool.GetElem(memid);
	if (player == nullptr) return nullptr;
	return player;
}

void LoginPlayerPool::DelLoginPlayer(LoginPlayer* player)
{
	if (player == nullptr) return;
	m_player_pool.DelElem(player->GetMemId());
}