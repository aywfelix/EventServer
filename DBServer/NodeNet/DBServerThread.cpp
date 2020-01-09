#include "NodeNet/DBServerThread.h"
#include "Util.h"

std::unique_ptr<DBServerThread> g_pServerThread = nullptr;
bool DBServerThread::Init()
{
    m_NodeServer.Init();
    m_NodeServer.InitHelper();
	return true;
}

void DBServerThread::ThreadLoop()
{
    while(IsActive())
    {
        m_NodeServer.Loop();
		m_NodeClient.Loop();
		SFSLEEP(LOOP_TIMEOUT);
    }
}
