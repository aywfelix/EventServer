#include "Login.h"
#include "NodeNet/LoginServerThread.h"
#include "designtable/TableMgr.h"
#include "Session.h"
#include "Connection/ConnectionPool.h"

Login::Login()
{

}
Login::~Login()
{

}

void Login::Init()
{
	g_pTableMgr = std::make_unique<TableMgr>();
	g_pServerThread = std::make_unique<LoginServerThread>();
	g_pSessionPool = std::make_unique<SessionPool>();
	g_conn_pool = std::make_unique<ConnectionPool>();

	InitManager();
}

void Login::Start()
{
	g_pServerThread->Start();
}

void Login::Stop()
{
	g_pServerThread->Stop();
	g_conn_pool->Stop();
}

void Login::InitManager()
{
	g_conn_pool->Init();
}