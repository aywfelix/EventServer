#include "MyServer.h"
#include "SeSelect.h"
#include "LogHelper.h"
#include "Session.h"

#include <set>
#include<iostream>
#include "Timer/TimerLoop.h"

using namespace std;

static void Print()
{
	cout << "test timer" << endl;
}

std::function<void()> P = Print;

int main()
{
	
	INIT_SFLOG(true, true);
	/*signal(SIGPIPE, SIG_IGN);*/
	//g_pSessionPool = std::make_unique<SessionPool>();
	//SeEventOp* pEventOp = new SeSelect;
	//pEventOp->Init();
	//seEventLoop server;
	//server.Init(pEventOp);
	//server.InitServer(88888);
	//server.StartLoop();
	//server.StopLoop();

	TimerLoop timerLoop;

	timerLoop.RunAfter(10 * 1000, P);


	while (1)
	{
		timerLoop.TimeLoop();
		SFSLEEP(100);
	}
	return 0;
}