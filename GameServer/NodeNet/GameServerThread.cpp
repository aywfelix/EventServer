#include "GameServerThread.h"
#include "Util.h"

std::unique_ptr<GameServerThread> g_pServerThread = nullptr;

bool GameServerThread::Init()
{
	m_game_client.Init();
	m_game_client.InitHelper();
	return true;
}

void GameServerThread::ThreadLoop()
{
	while (IsActive())
	{
		m_game_client.Loop();
		SFSLEEP(LOOP_TIMEOUT);
	}
	
}
