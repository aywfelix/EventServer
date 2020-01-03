#pragma once
#include <map>
#include <memory>
#include <list>
#include "SeFINet.h"
#include "MemPool.hpp"
#include "ClientPlayer.h"

class Session;

using clientplayer_pool_t = MemPool<ClientPlayer>;
class ClientPlayerMgr 
{
public:
	ClientPlayerMgr() {}
	virtual ~ClientPlayerMgr() {}
	ClientPlayer* NewPlayer(Session* pSession);
	void DelPlayer(ClientPlayer* pClientPlayer);
	ClientPlayer* GetPlayer(int memId);

	void AddPlayerIDMap(UINT64 playerId, ClientPlayer* pPlayer);
	ClientPlayer* GetPlayerByID(UINT64 playerId);

private:
	clientplayer_pool_t mClientPool;
	//SocketId ClientPlayer
	std::map<socket_t, ClientPlayer*> mPlayerSockMap;
	//PlayerId ClientPlayer
	std::map<UINT64, ClientPlayer*> mPlayerIDMap;
};

extern std::unique_ptr<ClientPlayerMgr> g_pClientPlayerMgr;
