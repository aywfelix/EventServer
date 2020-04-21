#include "DB.h"
#include "NodeNet/DBServerThread.h"
#include "Connection/ConnectionPool.h"
#include "Session.h"
#include "packet/PacketMgr.h"

DB::DB(){}
DB::~DB(){}

void DB::Init()
{
	g_pServerThread = std::make_unique<DBServerThread>();
	g_conn_pool = std::make_unique<ConnectionPool>();
	g_pSessionPool = std::make_unique<SessionPool>();
	g_pPacketMgr = std::make_unique<PacketMgr>();
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
	g_conn_pool->Init();
}
