#pragma once
#include "packet/Player.h"
#include "MemPool.hpp"

class ObjPlayer;

class GamePlayer : public Player
{
public:
	virtual bool Init();

	virtual bool HeartBeat();
	int SendToGate(const int msg_id, ::google::protobuf::Message* pb_msg);
	int SendToWorld(const int msg_id, ::google::protobuf::Message* pb_msg);
	void SetObjPlayer(ObjPlayer* player) { m_obj_player = player; }
	ObjPlayer* GetObjPlayer() { return m_obj_player; }
private:
	ObjPlayer* m_obj_player;
};

using gameplayer_pool_t = MemPool<GamePlayer>;
class GamePlayerPool
{
public:
	GamePlayerPool();
	~GamePlayerPool();
	GamePlayer* NewGamePlayer();
	GamePlayer* GetGamePlayer(int32_t memid);
	void DelGamePlayer(GamePlayer* player);
private:
	gameplayer_pool_t m_player_pool;
};

extern std::unique_ptr<GamePlayerPool> g_pGamePlayerPool;