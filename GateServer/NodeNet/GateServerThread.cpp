#include "NodeNet/GateServerThread.h"
#include "Util.h"
#include "SePlatForm.h"

std::unique_ptr<GateServerThread> g_pServerThread = nullptr;
bool GateServerThread::Init()
{
	m_gate_cli.Init();
	m_gate_cli.InitHelper();

    m_gate_serv.Init();
	m_gate_serv.InitHelper();

	m_gate_player.Init();

	return true;
}

void GateServerThread::ThreadLoop()
{
    while(IsActive())
    {
        m_gate_serv.Loop();
		m_gate_cli.Loop();
		m_gate_player.Loop();
		sf_sleep(LOOP_TIMEOUT);
    }
}
