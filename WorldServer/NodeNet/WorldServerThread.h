#pragma once
#include "ThreadBase.h"
#include "WorldNodeServer.h"
#include "WorldNodeClient.h"
#include <memory>

class WorldServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();
	
	WorldNodeServer& NodeServer() { return m_world_serv; }
	WorldNodeClient& NodeClient() { return m_world_cli; }
private:
    WorldNodeServer m_world_serv;
	WorldNodeClient m_world_cli;
};

extern std::unique_ptr<WorldServerThread> g_pServerThread;