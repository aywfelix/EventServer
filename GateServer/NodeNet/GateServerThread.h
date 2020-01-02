#pragma once
#include <memory>
#include "ThreadBase.h"
#include "NodeNet/GateNodeClient.h"
#include "NodeNet/GateNodeServer.h"
#include "NodeNet/GatePlayerServer.h"

class GateServerThread : public ThreadBase
{
public:
	bool Init();
	void ThreadLoop();

	GatePlayerServer &PlayerServer()
	{
		return m_PlayerServer;
	}

	GateNodeServer &NodeServer()
	{
		return m_NodeServer;
	}

	GateNodeClient &NodeClient()
	{
		return m_NodeClient;
	}

private:
	GatePlayerServer m_PlayerServer;
	GateNodeServer m_NodeServer;
	GateNodeClient m_NodeClient;
};

extern std::unique_ptr<GateServerThread> g_pServerThread;