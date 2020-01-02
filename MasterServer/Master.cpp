#include "Master.h"
#include "NodeNet/MasterServerThread.h"


Master::Master()
{

}
Master::~Master()
{

}

void Master::Init()
{
	g_pServerThread.reset(new MasterServerThread());

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
