#include "SePlatForm.h"
#include "MasterServerThread.h"

std::unique_ptr<MasterServerThread> g_pServerThread = nullptr;

bool MasterServerThread::Init()
{
    m_nodeserver.Init();
    m_nodeserver.InitHelper();
	return true;
}

void MasterServerThread::ThreadLoop()
{
    while(IsActive())
    {
        m_nodeserver.Loop();
		SFSLEEP(LOOP_TIMEOUT);
    }
}
