#pragma once
#include "ThreadBase.h"
#include "WorldNodeServer.h"
#include "WorldNodeClient.h"
#include <memory>

class WorldServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();
	
	WorldNodeServer& NodeServer()
	{
		return m_NodeServer;
	}

	WorldNodeClient& NodeClient()
	{
		return m_NodeClient;
	}

private:
    WorldNodeServer m_NodeServer;
	WorldNodeClient m_NodeClient;
};

extern std::unique_ptr<WorldServerThread> g_pServerThread;