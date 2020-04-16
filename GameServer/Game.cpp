#include "Game.h"
#include "NodeNet/GameServerThread.h"
#include "designtable/TableMgr.h"
#include "Session.h"
#include "redis/db_redis.h"
#include "scene/map/SceneMapMgr.h"

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
	g_pSceneMapMgr = std::make_unique<SceneMapMgr>();
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
	g_pRedis->init(g_pConfig->m_RedisConf["ConnNum"].asInt(),
		g_pConfig->m_RedisConf["ip"].asCString(),
		g_pConfig->m_RedisConf["port"].asInt());
	g_pSceneMapMgr->LoadAllMap();
}
