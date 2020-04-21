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

	GatePlayerServer &PlayerServer() { return m_gate_player; }
	GateNodeServer &NodeServer() { return m_gate_serv; }
	GateNodeClient &NodeClient() { return m_gate_cli; }

private:
	GatePlayerServer m_gate_player;
	GateNodeServer m_gate_serv;
	GateNodeClient m_gate_cli;
};

extern std::unique_ptr<GateServerThread> g_pServerThread;