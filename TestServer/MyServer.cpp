#include "MyServer.h"
#include "SeSelect.h"
#include "LogHelper.h"
#include "Session.h"

#include <set>
#include<iostream>

using namespace std;

int main()
{
	
	INIT_SFLOG(true, true);
	/*signal(SIGPIPE, SIG_IGN);*/
	multiset<string> ms;
	ms.insert("abc");
	ms.insert("123");
	ms.insert("111");
	ms.insert("aaa");
	ms.insert("123");
	ms.insert("123");
	ms.insert("123");
	ms.insert("bbb");

	multiset<string>::iterator it;
	for (it = ms.begin(); it != ms.end(); it++)
	{
		cout << *it << endl;
	}
	cout << endl << "集合的大小:" << ms.size() << endl;

	it = ms.find("123");
	while (it != ms.end() && *it == "123")
	{
		cout << *it << endl;
		it++;
	}

	it = ms.find("43");
	if (it != ms.end())
	{
		cout << *it << endl;
	}
	else cout << "not found" << endl;

	int n = ms.erase("123");
	cout << "共删除:" << n << endl << endl;
	for (it = ms.begin(); it != ms.end(); it++)
	{
		cout << *it << endl;
	}
	//g_pSessionPool = std::make_unique<SessionPool>();
	//SeEventOp* pEventOp = new SeSelect;
	//pEventOp->Init();
	//seEventLoop server;
	//server.Init(pEventOp);
	//server.InitServer(88888);
	//server.StartLoop();
	//server.StopLoop();

	while (1)
	{
		SFSLEEP(100);
	}
	return 0;
}