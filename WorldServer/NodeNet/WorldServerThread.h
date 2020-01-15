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
		return m_nodeserver;
	}

	WorldNodeClient& NodeClient()
	{
		return m_nodeclient;
	}

private:
    WorldNodeServer m_nodeserver;
	WorldNodeClient m_nodeclient;
};

extern std::unique_ptr<WorldServerThread> g_pServerThread;