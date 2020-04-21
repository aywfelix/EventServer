#include "Gate.h"
#include "NodeNet/GateServerThread.h"
#include "ClientPlayer/ClientPlayerMgr.h"
#include "Session.h"
#include "SnowFlake.h"
#include "packet/PacketMgr.h"

Gate::Gate()
{

}
Gate::~Gate()
{

}

void Gate::Init()
{
	g_pServerThread = std::make_unique<GateServerThread>();
	g_pClientPlayerMgr = std::make_unique<ClientPlayerMgr>();
	g_pSessionPool = std::make_unique<SessionPool>();
	g_pSnowFlake = std::make_unique<SnowFlake>();
	g_pPacketMgr = std::make_unique<PacketMgr>();
	InitManager();
}

void Gate::Start()
{
	g_pServerThread->Start();
}

void Gate::Stop()
{
	g_pServerThread->Stop();
}

void Gate::InitManager()
{
	g_pSnowFlake->Init();
	// register msg
}
