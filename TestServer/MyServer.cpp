#include "MyServer.h"
#include "SeSelect.h"
#include "LogHelper.h"
#include "Session.h"

#include <set>
#include<iostream>
#include "Timer/TimerLoop.h"

using namespace std;

void Print()
{
	cout << "test timer" << endl;
}


int main()
{
	
	INIT_SFLOG(true);
	/*signal(SIGPIPE, SIG_IGN);*/
	g_pSessionPool = std::make_unique<SessionPool>();
	seEventLoop server;
	server.Init();
	server.InitServer(88888);
	server.StartLoop();
	server.StopLoop();

	TimerLoop timerLoop;

	timerLoop.RunAfter(10 * 1000, Print);
	timerLoop.RunAfter(15 * 1000, Print);
	timerLoop.RunAfter(20 * 1000, Print);

	TimeId tid = timerLoop.NearestTimer();
	cout << tid.GetSeq() << endl;

	while (1)
	{
		TimeId tid = timerLoop.NearestTimer();
		cout << tid.GetSeq() << endl;
		timerLoop.TimeLoop();
		SFSLEEP(100);
	}
	return 0;
}