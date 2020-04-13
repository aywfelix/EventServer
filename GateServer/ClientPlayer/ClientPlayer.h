#pragma once

#include <memory>
#include "packet/Player.h"
#include "SocketDefine.h"

class Session;

class ClientPlayer : public Player
{
public:

	virtual bool Init();
	virtual bool Clear();
	socket_t GetSockFd();
	uint64_t GetPlayerId() { return m_playerid; }   // Ä¬ÈÏplayeridÎªmemid
	void SetSceneId(int sceneId) { m_sceneid = sceneId; }
	void SetLastSceneId(int sceneId) { m_lastsceneid = sceneId; }
	void SetSession(Session* pSession) { m_session = pSession; }
	void SetConnTime(int64_t ti) { m_conntime = ti; }
	void SetPlayerId(uint64_t playerId) { m_playerid = playerId; }
public:
	bool SendToGate(const int msg_id, const char* msg, size_t msg_len, socket_t sock_fd);
	bool SendToLogin(const int msg_id, const char* msg, size_t msg_len);
	bool SendToGame(const int msg_id, const char* msg, size_t msg_len);
	bool SendToChat(const int msg_id, const char* msg, size_t msg_len);
	bool SendToWorld(const int msg_id, const char* msg, size_t msg_len);
	void SendToClient(const int msg_id, const std::string& msg);
private:
	int m_lineid;
	int m_sceneid;
	int m_lastsceneid;
	int64_t m_conntime;
	Session* m_session;
};

