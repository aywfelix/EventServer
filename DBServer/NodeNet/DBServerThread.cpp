#include "NodeNet/DBServerThread.h"
#include "Util.h"

std::unique_ptr<DBServerThread> g_pServerThread = nullptr;
bool DBServerThread::Init()
{
    m_nodeserver.Init();
    m_nodeserver.InitHelper();
	return true;
}

void DBServerThread::ThreadLoop()
{
    while(IsActive())
    {
        m_nodeserver.Loop();
		m_nodeclient.Loop();
		SFSLEEP(LOOP_TIMEOUT);
    }
}
