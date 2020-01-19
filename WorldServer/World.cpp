#include "World.h"
#include "NodeNet/WorldServerThread.h"


World::World()
{

}
World::~World()
{

}

void World::Init()
{
	g_pServerThread.reset(new WorldServerThread());

	InitManager();
}

void World::Start()
{
	g_pServerThread->Start();
}

void World::Stop()
{
	g_pServerThread->Stop();
}

void World::InitManager()
{

}
