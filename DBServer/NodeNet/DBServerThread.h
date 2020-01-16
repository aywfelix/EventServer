#pragma once
#include "ThreadBase.h"
#include "DBNodeServer.h"
#include "DBNodeClient.h"
#include <memory>

class DBServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();
	
	DBNodeServer& NodeServer() { return m_db_serv; }
	DBNodeClient& NodeClient() { return m_db_cli; }
private:
    DBNodeServer m_db_serv;
	DBNodeClient m_db_cli;
};

extern std::unique_ptr<DBServerThread> g_pServerThread;