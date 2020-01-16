#include "LoginServerThread.h"
#include "Util.h"

std::unique_ptr<LoginServerThread> g_pServerThread = nullptr;

bool LoginServerThread::Init()
{
	m_login_cli.Init();
	m_login_cli.InitHelper();
	return true;
}

void LoginServerThread::ThreadLoop()
{
	while (IsActive())
	{
		m_login_cli.Loop();
		SFSLEEP(LOOP_TIMEOUT);
	}
	
}
