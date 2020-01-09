#include "GameServerThread.h"
#include "Util.h"

std::unique_ptr<GameServerThread> g_pServerThread = nullptr;

bool GameServerThread::Init()
{
	m_GameClient.Init();
	m_GameClient.InitHelper();
	return true;
}

void GameServerThread::ThreadLoop()
{
	while (IsActive())
	{
		m_GameClient.Loop();
		SFSLEEP(LOOP_TIMEOUT);
	}
	
}
