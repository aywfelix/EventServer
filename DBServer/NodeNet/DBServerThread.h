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
		return m_nodeserver;
	}

	DBNodeClient& NodeClient()
	{
		return m_nodeclient;
	}

private:
    DBNodeServer m_nodeserver;
	DBNodeClient m_nodeclient;

};

extern std::unique_ptr<DBServerThread> g_pServerThread;