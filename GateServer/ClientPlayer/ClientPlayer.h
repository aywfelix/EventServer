#pragma once


class ClientPlayer
{
public:
	ClientPlayer(int id);
	ClientPlayer();
	virtual ~ClientPlayer();

	void SetSceneId(int sceneId) { m_SceneId = sceneId; }
	void SetLastSceneId(int sceneId) { m_LastSceneId = sceneId; }
	void SetNetObject(NetObject* pObject) { m_pNetObject = pObject; }
	void SetConnTime(uint64_t ti) { m_ConnTime = ti; }
	void SetPlayerId(uint64_t playerId) { m_PlayerId = playerId; }

	void Clear();

	NFSOCK GetSockFd();
	uint64_t GetPlayerId() { return m_PlayerId; }

	int GetId() { return m_id; }

public:
	bool OnModuleGateMessage(const int msgid, const char* msg, uint32_t nLen, NFSOCK nSockIndex);
	bool OnModuleLoginMessage(const int msgid, const char* msg, uint32_t nLen);
	bool OnModuleGameMessage(const int msgid, const char* msg, uint32_t nLen);
	bool OnModuleChatMessage(const int msgid, const char* msg, uint32_t nLen);
	bool OnModuleWorldMessage(const int msgid, const char* msg, uint32_t nLen);

	void SendToClient(const int nMsgID, const std::string& msg);
private:
    NetObject *m_pNetObject; //store player socket info
    int m_SceneId;
    int m_LastSceneId;
    int m_LineId;
    uint64_t m_ConnTime; 
	uint64_t m_PlayerId;

	int m_id;
};