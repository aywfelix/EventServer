#pragma once

#include <memory>
#include "MemPool.hpp"
#include "SocketDefine.h"

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
	uint64_t GetPlayerId() { return mPlayerId; }   // Ä¬ÈÏplayeridÎªmemid
	void SetSceneId(int sceneId) { mSceneId = sceneId; }
	void SetLastSceneId(int sceneId) { mLastSceneId = sceneId; }
	void SetSession(Session* pSession) { mSession = pSession; }
	void SetConnTime(int64_t ti) { mConnTime = ti; }
	void SetPlayerId(uint64_t playerId) { mPlayerId = playerId; }
public:
	bool OnModuleGateMessage(const int msgid, const char* msg, uint32_t nLen, socket_t nFd);
	bool OnModuleLoginMessage(const int msgid, const char* msg, uint32_t nLen);
	bool OnModuleGameMessage(const int msgid, const char* msg, uint32_t nLen);
	bool OnModuleChatMessage(const int msgid, const char* msg, uint32_t nLen);
	bool OnModuleWorldMessage(const int msgid, const char* msg, uint32_t nLen);
	void SendToClient(const int nMsgID, const std::string& msg);
private:
	uint64_t mPlayerId;
	int mLineId;
	int mSceneId;
	int mLastSceneId;
	int64_t mConnTime;
	Session* mSession;
};

