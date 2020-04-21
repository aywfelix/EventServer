#pragma once

#include "packet/Player.h"

class LoginPlayer : public Player
{
public:
	int SendToGate(unsigned short msg_id, ::google::protobuf::Message* pb_msg);
	int SendToWorld(unsigned short msg_id, ::google::protobuf::Message* pb_msg);

};

using loginplayer_pool_t = MemPool<LoginPlayer>;
class LoginPlayerPool
{
public:
	LoginPlayerPool();
	~LoginPlayerPool();
	LoginPlayer* NewLoginPlayer();
	LoginPlayer* GetLoginPlayer(int32_t memid);
	void DelLoginPlayer(LoginPlayer* player);
private:
	loginplayer_pool_t m_player_pool;
};

extern std::unique_ptr<LoginPlayerPool> g_pLoginPlayerPool;