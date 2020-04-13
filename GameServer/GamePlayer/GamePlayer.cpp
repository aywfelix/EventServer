#include "GamePlayer.h"
#include "obj/ObjPlayer.h"
#include "NodeNet/GameServerThread.h"

std::unique_ptr<GamePlayerPool> g_pGamePlayerPool = nullptr;

bool GamePlayer::Init()
{
	return true;
}

bool GamePlayer::HeartBeat()
{
	return true;
}

int GamePlayer::SendToGate(const int msg_id, ::google::protobuf::Message* pb_msg)
{
	g_pServerThread->GameClient().SendToGate(m_servid, m_playerid, msg_id, pb_msg);
	return 0;
}
int GamePlayer::SendToWorld(const int msg_id, ::google::protobuf::Message* pb_msg)
{
	g_pServerThread->GameClient().SendToWorld(m_servid, m_playerid, msg_id, pb_msg);
	return 0;
}

GamePlayerPool::GamePlayerPool() {}
GamePlayerPool::~GamePlayerPool() {}

GamePlayer* GamePlayerPool::NewGamePlayer()
{
	GamePlayer* player = m_player_pool.NewElem();
	if (player == nullptr) return nullptr;
	return player;
}

GamePlayer* GamePlayerPool::GetGamePlayer(int32_t memid)
{
	GamePlayer* player = m_player_pool.GetElem(memid);
	if (player == nullptr) return nullptr;
	return player;
}

void GamePlayerPool::DelGamePlayer(GamePlayer* player)
{
	if (player == nullptr) return;
	m_player_pool.DelElem(player->GetMemId());
}