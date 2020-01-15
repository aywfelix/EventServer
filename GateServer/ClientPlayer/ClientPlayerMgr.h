#pragma once
#include <map>
#include <memory>
#include <list>
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
	void DelPlayer(ClientPlayer* player);
	ClientPlayer* GetPlayer(int memId);

	void AddPlayerIDMap(uint64_t playerId, ClientPlayer* player);
	ClientPlayer* GetPlayerByID(uint64_t playerId);

	void AddPlayerSockMap(socket_t nFd, ClientPlayer* player);
	ClientPlayer* GetPlayerByFd(socket_t nFd);

private:
	clientplayer_pool_t mClientPool;
	//SocketId ClientPlayer
	std::map<socket_t, ClientPlayer*> mPlayerSockMap;
	//PlayerId ClientPlayer
	std::map<uint64_t, ClientPlayer*> mPlayerIDMap;
};

extern std::unique_ptr<ClientPlayerMgr> g_pClientPlayerMgr;
