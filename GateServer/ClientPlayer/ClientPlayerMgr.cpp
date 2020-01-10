#include "ClientPlayerMgr.h"
#include "ClientPlayer.h"
#include "Session.h"

std::unique_ptr<ClientPlayerMgr> g_pClientPlayerMgr = nullptr;


ClientPlayer* ClientPlayerMgr::NewPlayer(Session* pSession)
{
	if (pSession == nullptr) return nullptr;

	ClientPlayer* pPlayer = mClientPool.NewElem();
	if (pPlayer == nullptr) return nullptr;
	pPlayer->SetSession(pSession);
	pPlayer->SetPlayerId(pPlayer->GetMemId());
	return pPlayer;
}

void ClientPlayerMgr::DelPlayer(ClientPlayer* player)
{
	mClientPool.DelElem(player->GetId());
	mPlayerSockMap.erase(player->GetSockFd());
	mPlayerIDMap.erase(player->GetPlayerId());
}

ClientPlayer* ClientPlayerMgr::GetPlayer(int memId)
{
	return mClientPool.GetElem(memId);
}

void ClientPlayerMgr::AddPlayerIDMap(UINT64 playerId, ClientPlayer* player)
{
	if (player == nullptr) return;

	auto it = mPlayerIDMap.find(playerId);
	if (it == mPlayerIDMap.end())
	{
		mPlayerIDMap.emplace(playerId, player);
	}
}
ClientPlayer* ClientPlayerMgr::GetPlayerByID(UINT64 playerId)
{
	auto it = mPlayerIDMap.find(playerId);
	if (it == mPlayerIDMap.end()) return nullptr;

	return it->second;
}

void ClientPlayerMgr::AddPlayerSockMap(socket_t nFd, ClientPlayer* player)
{
	if (player == nullptr) return;
	auto it = mPlayerSockMap.find(nFd);
	if (it == mPlayerSockMap.end())
	{
		mPlayerSockMap.emplace(nFd, player);
	}
}

ClientPlayer* ClientPlayerMgr::GetPlayerByFd(socket_t nFd)
{
	auto it = mPlayerSockMap.find(nFd);
	if (it == mPlayerSockMap.end()) return nullptr;

	return it->second;
}