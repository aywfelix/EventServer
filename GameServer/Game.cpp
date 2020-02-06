#include "Game.h"
#include "NodeNet/GameServerThread.h"
#include "designtable/TableMgr.h"
#include "Session.h"

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
}
