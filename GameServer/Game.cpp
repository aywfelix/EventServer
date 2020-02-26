#include "Game.h"
#include "NodeNet/GameServerThread.h"
#include "designtable/TableMgr.h"
#include "Session.h"
#include "redis/db_redis.h"

Game::Game()
{

}
Game::~Game()
{

}

void Game::Init()
{
	g_pTableMgr = std::make_unique<TableMgr>();
	g_pServerThread = std::make_unique<GameServerThread>();
	g_pSessionPool = std::make_unique<SessionPool>();
	g_pRedis = std::make_unique<db_redis>();
	InitManager();
}

void Game::Start()
{
	g_pServerThread->Start();
}

void Game::Stop()
{
	g_pServerThread->Stop();
}

void Game::InitManager()
{
	//load tables
	g_pTableMgr->LoadTables();
	g_pRedis->init(g_JsonConfig->m_RedisConf["ConnNum"].asInt(),
		g_JsonConfig->m_RedisConf["ip"].asCString(),
		g_JsonConfig->m_RedisConf["port"].asInt());
}
