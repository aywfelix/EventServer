#pragma once
#include "ThreadBase.h"
#include "GameNodeClient.h"


class GameServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();

	GameNodeClient& GameClient() { return m_game_client; }
private:
	GameNodeClient m_game_client;
};

extern std::unique_ptr<GameServerThread> g_pServerThread;