#include "Gate.h"
#include "NodeNet/GateServerThread.h"
#include "ClientPlayer/ClientPlayerMgr.h"

Gate::Gate()
{

}
Gate::~Gate()
{

}

void Gate::Init()
{
	g_pServerThread.reset(new GateServerThread());
	g_pClientPlayerMgr.reset(new ClientPlayerMgr);
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
}
