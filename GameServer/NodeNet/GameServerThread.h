#pragma once
#include "ThreadBase.h"
#include "GameNodeClient.h"


class GameServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();

	GameNodeClient& GameClient()
	{
		return m_GameClient;
	}
private:
	GameNodeClient m_GameClient;
};

extern std::unique_ptr<GameServerThread> g_pServerThread;