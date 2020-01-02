#include "ClientPlayer/ClientPlayerMgr.h"
#include "ClientPlayer/ClientPlayer.h"
#include "NFNetPlugin/NFINet.h"


std::unique_ptr<ClientPlayerMgr> g_pClientPlayerMgr = nullptr;

const int MAX_PLAYER = 5000;

ClientPlayerMgr::ClientPlayerMgr()
{
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		ClientPlayer* pPlayer = new ClientPlayer(i + 1);   //TODO use mempool
		mNonPlayerList.push_back(pPlayer);
	}
}

ClientPlayerMgr::~ClientPlayerMgr()
{
	Destory();
}


void ClientPlayerMgr::Destory()
{
	auto idIter = mPlayerIDMap.begin();
	for (; idIter != mPlayerIDMap.end(); ++idIter)
	{
		if (idIter->second)
		{
			idIter->second->Clear();
		}
	}
	mPlayerIDMap.clear();

	auto listIter = mNonPlayerList.begin();
	for (; listIter != mNonPlayerList.end(); ++listIter)
	{
		delete* listIter;
	}

	mNonPlayerList.clear();

	auto sockIter = mPlayerSockMap.begin();
	for (; sockIter != mPlayerSockMap.end(); ++sockIter)
	{
		delete sockIter->second;
	}
	mPlayerSockMap.clear();


}

ClientPlayer* ClientPlayerMgr::CreatePlayer(NetObject* pNetObject)
{
	if (pNetObject == nullptr)
		return nullptr;

	if (mNonPlayerList.size() <= 0)
	{
		//Client Player full, Obj Used Out!
		return nullptr;
	}
	auto iter = mNonPlayerList.begin();
	ClientPlayer* pPlayer = *iter;
	mNonPlayerList.pop_front();

	pPlayer->Clear();
	pPlayer->SetNetObject(pNetObject);
	mPlayerSockMap[pNetObject->GetRealFD()] = pPlayer;

	return pPlayer;
}

void ClientPlayerMgr::DestoryPlayer(ClientPlayer* pPlayer)
{
	if (pPlayer == nullptr)
		return;

	auto iter = mPlayerSockMap.find(pPlayer->GetSockFd());
	if (iter != mPlayerSockMap.end())
	{
		mPlayerSockMap.erase(iter);
	}

	pPlayer->SetNetObject(nullptr);

	auto iditer = mPlayerIDMap.find(pPlayer->GetPlayerId());
	if (iditer != mPlayerIDMap.end())
	{
		if (pPlayer == iditer->second)
		{
			mPlayerIDMap.erase(iditer);
		}
	}

	pPlayer->Clear();
	mNonPlayerList.push_back(pPlayer);
}


ClientPlayer* ClientPlayerMgr::GetPlayer(NFSOCK nSockIndex)
{
	auto iter = mPlayerSockMap.find(nSockIndex);
	if (iter == mPlayerSockMap.end())
		return nullptr;

	return iter->second;
}

void ClientPlayerMgr::AddPlayerIDMap(uint64_t playerId, ClientPlayer* pPlayer)
{
	if (pPlayer == nullptr)
	{
		return;
	}

	auto iter = mPlayerIDMap.find(playerId);
	if (iter != mPlayerIDMap.end())
	{
		if (iter->second == pPlayer)
			return;

		mPlayerIDMap.erase(iter);
	}

	pPlayer->SetPlayerId(playerId);
	mPlayerIDMap[playerId] = pPlayer;
}

ClientPlayer* ClientPlayerMgr::GetPlayerByID(uint64_t playerId)
{
	auto iter = mPlayerIDMap.find(playerId);
	if (iter == mPlayerIDMap.end())
		return nullptr;

	return iter->second;
}