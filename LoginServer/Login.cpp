#include "Login.h"
#include "NodeNet/LoginServerThread.h"
#include "DesignTable/TableMgr.h"
#include "Session.h"

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
}

void Login::Start()
{
	g_pServerThread->Start();
}

void Login::Stop()
{
	g_pServerThread->Stop();
}
