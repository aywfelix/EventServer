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
	bool OnModuleGateMessage(const int msg_id, const char* msg, uint32_t msg_len, socket_t sock_fd);
	bool OnModuleLoginMessage(const int msg_id, const char* msg, uint32_t msg_len);
	bool OnModuleGameMessage(const int msg_id, const char* msg, uint32_t msg_len);
	bool OnModuleChatMessage(const int msg_id, const char* msg, uint32_t msg_len);
	bool OnModuleWorldMessage(const int msg_id, const char* msg, uint32_t msg_len);
	void SendToClient(const int msg_id, const std::string& msg);
private:
	uint64_t mPlayerId;
	int mLineId;
	int mSceneId;
	int mLastSceneId;
	int64_t mConnTime;
	Session* mSession;
};

