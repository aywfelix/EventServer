#include "Master.h"
#include "NodeNet/MasterServerThread.h"
#include "Session.h"

Master::Master()
{

}
Master::~Master()
{

}

void Master::Init()
{
	g_pServerThread = std::make_unique<MasterServerThread>();
	g_pSessionPool = std::make_unique<SessionPool>();
	InitManager();
}

void Master::Start()
{
	g_pServerThread->Start();
}

void Master::Stop()
{
	g_pServerThread->Stop();
}

void Master::InitManager()
{

}
