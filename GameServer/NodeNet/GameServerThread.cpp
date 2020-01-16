#include "GameServerThread.h"
#include "Util.h"

std::unique_ptr<GameServerThread> g_pServerThread = nullptr;

bool GameServerThread::Init()
{
	m_game_cli.Init();
	m_game_cli.InitHelper();
	return true;
}

void GameServerThread::ThreadLoop()
{
	while (IsActive())
	{
		m_game_cli.Loop();
		SFSLEEP(LOOP_TIMEOUT);
	}
	
}
