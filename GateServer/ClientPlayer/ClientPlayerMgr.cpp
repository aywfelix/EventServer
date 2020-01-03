#include "ClientPlayerMgr.h"
#include "ClientPlayer.h"
#include "Session.h"

std::unique_ptr<ClientPlayerMgr> g_pClientPlayerMgr = nullptr;


ClientPlayer* ClientPlayerMgr::NewPlayer(Session* pSession)
{
	ClientPlayer* pPlayer = mClientPool.NewElem();
	if (pPlayer == nullptr)
		return nullptr;
	pPlayer->SetSession(pSession);
	return pPlayer;
}

void ClientPlayerMgr::DelPlayer(ClientPlayer* pClientPlayer)
{
	mClientPool.DelElem(pClientPlayer->GetId());
}

ClientPlayer* ClientPlayerMgr::GetPlayer(int memId)
{
	return mClientPool.GetElem(memId);
}

void ClientPlayerMgr::AddPlayerIDMap(UINT64 playerId, ClientPlayer* pPlayer)
{

}
ClientPlayer* ClientPlayerMgr::GetPlayerByID(UINT64 playerId)
{
	return nullptr;
}