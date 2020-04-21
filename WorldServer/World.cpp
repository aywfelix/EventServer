#include "World.h"
#include "NodeNet/WorldServerThread.h"
#include "Session.h"
#include "client/world/HandleWorld.h"
#include "packet/PacketMgr.h"

World::World()
{

}
World::~World()
{

}

void World::Init()
{
	g_pServerThread = std::make_unique<WorldServerThread>();
	g_pSessionPool = std::make_unique<SessionPool>();
	g_pPacketMgr = std::make_unique<PacketMgr>();
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
