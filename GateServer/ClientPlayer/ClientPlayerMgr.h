
#pragma once
#include <map>
#include <memory>
#include <list>


class ClientPlayer;
class NetObject;

class ClientPlayerMgr
{
public:
	ClientPlayerMgr();
	virtual ~ClientPlayerMgr();

	void Destory();

	ClientPlayer* CreatePlayer(NetObject* pNetObject);
	void DestoryPlayer(ClientPlayer* pPlayer);

	ClientPlayer* GetPlayer(NFSOCK nSockIndex);

	void AddPlayerIDMap(uint64_t playerId, ClientPlayer* pPlayer);

	ClientPlayer* GetPlayerByID(uint64_t playerId);

private:
	std::list<ClientPlayer*> mNonPlayerList;
	//SocketId ClientPlayer
	std::map<NFSOCK, ClientPlayer*> mPlayerSockMap;
	//PlayerId ClientPlayer
	std::map<uint64_t, ClientPlayer*> mPlayerIDMap;

};

extern std::unique_ptr<ClientPlayerMgr> g_pClientPlayerMgr;