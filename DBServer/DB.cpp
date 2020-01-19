#include "DB.h"
#include "NodeNet/DBServerThread.h"
#include "Connection/ConnectionPool.h"

DB::DB(){}
DB::~DB(){}

void DB::Init()
{
	g_pServerThread.reset(new DBServerThread());
	g_conn_pool.reset(new ConnectionPool);
	InitManager();
}

void DB::Start()
{
	g_pServerThread->Start();
}

void DB::Stop()
{
	g_pServerThread->Stop();
	g_conn_pool->Stop();
}

void DB::InitManager()
{

}
