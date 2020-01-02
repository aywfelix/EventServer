#pragma once

#include "ThreadBase.h"
#include "MasterNodeServer.h"


class MasterServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();
private:
    MasterNodeServer m_NodeServer;
};

extern std::unique_ptr<MasterServerThread> g_pServerThread;
