#include "NodeNet/GateServerThread.h"
#include "Util.h"
#include "SePlatForm.h"

std::unique_ptr<GateServerThread> g_pServerThread = nullptr;
bool GateServerThread::Init()
{
	m_nodeclient.Init();
	m_nodeclient.InitHelper();

    m_nodeserver.Init();
    m_nodeserver.InitHelper();

	m_playerserver.Init();

	return true;
}

void GateServerThread::ThreadLoop()
{
    while(IsActive())
    {
        m_nodeserver.Loop();
		m_nodeclient.Loop();
		m_playerserver.Loop();
		SFSLEEP(LOOP_TIMEOUT);
    }
}
