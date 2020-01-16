#include "LoginServerThread.h"
#include "Util.h"

std::unique_ptr<LoginServerThread> g_pServerThread = nullptr;

bool LoginServerThread::Init()
{
	m_game_client.Init();
	m_game_client.InitHelper();
	return true;
}

void LoginServerThread::ThreadLoop()
{
	while (IsActive())
	{
		m_game_client.Loop();
		SFSLEEP(LOOP_TIMEOUT);
	}
	
}
