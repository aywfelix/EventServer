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

	GatePlayerServer &PlayerServer() { return m_playerserver; }

	GateNodeServer &NodeServer() { return m_nodeserver; }

	GateNodeClient &NodeClient() { return m_nodeclient; }

private:
	GatePlayerServer m_playerserver;
	GateNodeServer m_nodeserver;
	GateNodeClient m_nodeclient;
};

extern std::unique_ptr<GateServerThread> g_pServerThread;