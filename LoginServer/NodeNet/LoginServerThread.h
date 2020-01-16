#pragma once
#include "ThreadBase.h"
#include "LoginNodeClient.h"


class LoginServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();

	LoginNodeClient& LoginClient() { return m_game_client; }
private:
	LoginNodeClient m_game_client;
};

extern std::unique_ptr<LoginServerThread> g_pServerThread;