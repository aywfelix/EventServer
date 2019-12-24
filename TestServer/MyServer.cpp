#include "MyServer.h"
#include "SeSelect.h"
#include "LogHelper.h"
#include "Session.h"

int main()
{
	g_pSessionPool = std::make_unique<SessionPool>();
	INIT_SFLOG(true, true);
	seEventLoop server;
	server.Init();
	SeEventOp* pEventOp = new SeSelect;
	pEventOp->Init(&server, 5);
	pEventOp->InitServer(88888);
	server.SetEventOp(pEventOp);
	server.StartLoop();
	while (1)
	{
		SFSLEEP(100);
	}
	return 0;
}