#pragma once
#include "ThreadBase.h"
#include "LoginNodeClient.h"


class LoginServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();

	LoginNodeClient& LoginClient() { return m_login_cli; }
private:
	LoginNodeClient m_login_cli;
};

extern std::unique_ptr<LoginServerThread> g_pServerThread;