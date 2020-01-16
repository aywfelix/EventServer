#include "NodeNet/DBServerThread.h"
#include "Util.h"

std::unique_ptr<DBServerThread> g_pServerThread = nullptr;
bool DBServerThread::Init()
{
    m_db_serv.Init();
	m_db_serv.InitHelper();

	m_db_cli.Init();
	m_db_cli.InitHelper();
	return true;
}

void DBServerThread::ThreadLoop()
{
    while(IsActive())
    {
        m_db_serv.Loop();
		m_db_cli.Loop();
		SFSLEEP(LOOP_TIMEOUT);
    }
}
