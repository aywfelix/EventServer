#include "NodeNet/WorldServerThread.h"
#include "Util.h"

std::unique_ptr<WorldServerThread> g_pServerThread = nullptr;
bool WorldServerThread::Init()
{
    m_nodeserver.Init();
    m_nodeserver.InitHelper();
	return true;
}

void WorldServerThread::ThreadLoop()
{
    while(IsActive())
    {
        m_nodeserver.Loop();
		SFSLEEP(LOOP_TIMEOUT);
    }
}
