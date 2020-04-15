#include "World.h"
#include "NodeNet/WorldServerThread.h"
#include "Session.h"
#include "client/world/HandleWorld.h"

World::World()
{

}
World::~World()
{

}

void World::Init()
{
	g_pServerThread.reset(new WorldServerThread());
	g_pSessionPool = std::make_unique<SessionPool>();
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
	// register msg
	HandleWorld::Instance();
}
