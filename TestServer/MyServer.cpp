#include "MyServer.h"
#include "SeSelect.h"
#include "LogHelper.h"
#include "Session.h"

int main()
{
	g_pSessionPool = std::make_unique<SessionPool>();
	INIT_SFLOG(true, true);
	SeEventOp* pEventOp = new SeSelect;
	pEventOp->Init();
	seEventLoop server;
	server.Init(pEventOp);
	server.InitServer(88888);
	server.StartLoop();
	server.StopLoop();
	while (1)
	{
		SFSLEEP(100);
	}
	return 0;
}