#pragma once
#include "ThreadBase.h"
#include "DBNodeServer.h"
#include "DBNodeClient.h"
#include <memory>

class DBServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();
	
	DBNodeServer& NodeServer()
	{
		return m_NodeServer;
	}

	DBNodeClient& NodeClient()
	{
		return m_NodeClient;
	}

private:
    DBNodeServer m_NodeServer;
	DBNodeClient m_NodeClient;

};

extern std::unique_ptr<DBServerThread> g_pServerThread;