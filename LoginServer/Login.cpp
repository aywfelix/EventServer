#include "Login.h"
#include "NodeNet/LoginServerThread.h"
#include "designtable/TableMgr.h"
#include "Session.h"
#include "Connection/ConnectionPool.h"
#include "redis/db_redis.h"
#include "login/HandleLogin.h"


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
	g_pRedis = std::make_unique<db_redis>();
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
	g_pRedis->init(g_JsonConfig->m_RedisConf["ConnNum"].asInt(), 
		g_JsonConfig->m_RedisConf["ip"].asCString(), 
		g_JsonConfig->m_RedisConf["port"].asInt());

	// register msg
	HandleLogin::Instance();
}