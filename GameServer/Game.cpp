#include "Game.h"
#include "NodeNet/GameServerThread.h"
#include "DesignTable/TableMgr.h"

Game::Game()
{

}
Game::~Game()
{

}

void Game::Init()
{
	g_pServerThread.reset(new GameServerThread());
	g_pTableMgr.reset(new TableMgr);
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
