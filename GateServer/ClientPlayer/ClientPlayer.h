#pragma once
#include "SePlatForm.h"
#include <memory>
#include "MemPool.hpp"

class Session;

class ClientPlayer : public MemElem
{
public:
	ClientPlayer(){}
	~ClientPlayer(){}
	virtual bool Init();
	virtual bool Clear();
	int GetId();
	socket_t GetSockFd();
	UINT64 GetPlayerId() { return mPlayerId; }
	void SetSceneId(int sceneId) { mSceneId = sceneId; }
	void SetLastSceneId(int sceneId) { mLastSceneId = sceneId; }
	void SetSession(Session* pSession) { mSession = pSession; }
	void SetConnTime(INT64 ti) { mConnTime = ti; }
	void SetPlayerId(UINT64 playerId) { mPlayerId = playerId; }
public:
	bool OnModuleGateMessage(const int msgid, const char* msg, uint32_t nLen, socket_t nFd);
	bool OnModuleLoginMessage(const int msgid, const char* msg, uint32_t nLen);
	bool OnModuleGameMessage(const int msgid, const char* msg, uint32_t nLen);
	bool OnModuleChatMessage(const int msgid, const char* msg, uint32_t nLen);
	bool OnModuleWorldMessage(const int msgid, const char* msg, uint32_t nLen);
	void SendToClient(const int nMsgID, const std::string& msg);
private:
	UINT64 mPlayerId;
	int mLineId;
	int mSceneId;
	int mLastSceneId;
	INT64 mConnTime;
	Session* mSession;
};

