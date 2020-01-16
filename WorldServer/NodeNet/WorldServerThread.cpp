#include "NodeNet/WorldServerThread.h"
#include "Util.h"

std::unique_ptr<WorldServerThread> g_pServerThread = nullptr;
bool WorldServerThread::Init()
{
    m_world_serv.Init();
	m_world_serv.InitHelper();

	m_world_cli.Init();
	m_world_cli.InitHelper();
	return true;
}

void WorldServerThread::ThreadLoop()
{
    while(IsActive())
    {
        m_world_serv.Loop();
		m_world_cli.Loop();
		SFSLEEP(LOOP_TIMEOUT);
    }
}
