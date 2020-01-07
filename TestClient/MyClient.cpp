#include "MyClient.h"
#include "SeSelect.h"
#include "LogHelper.h"
#include "Session.h"
#include "Util.h"
#include <iostream>
#include <sstream>
using namespace std;

void RecvCb(const socket_t nFd, const int nMsgId, const char* pMsg, const uint32_t nLen)
{
	cout << pMsg << endl;
}

void EventCb(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	cout << nFd << endl;
	cout << nEvent << endl;
}

//class MyStream 
//{
//public:
//	template<typename T>
//	MyStream& operator<<(const T& log)
//	{
//		moss << log;
//		return *this;
//	}
//
//	MyStream& operator<<(std::ostream& (*log)(std::ostream&))
//	{
//		moss << log;
//		std::ostringstream moss2;
//		moss.swap(moss2);
//		std::cout << moss2.str();
//		return *this;
//	}
//private:
//	std::ostringstream moss;
//	
//};

int main()
{
	//MyStream stream;
	//stream << "aaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbb";
	//stream << "aaaaaaaaaaaaaaa" << std::endl;
	g_pSessionPool = std::make_unique<SessionPool>();
	INIT_SFLOG("TestClient");
	CLOG_DEBUG << "aaaaaaaaaaaaaaaa" << CLOG_END;

	std::cout << GetTimeS() << std::endl;
	std::cout << time(0) << std::endl;
	std::cout << (int)(GetTimeS()/60) << std::endl;

	NET_RECEIVE_FUNCTOR Refunctor = std::bind(RecvCb, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	NET_EVENT_FUNCTOR Evfunctor = std::bind(EventCb, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	SeNet client(Refunctor, Evfunctor);

	client.InitClient("192.168.60.49", 88888);
	client.SendProtoMsg(111, "1234567890", 10);
	/*client.SendMsg("1111111111", 10);*/
	
	//client.InitClient("192.168.43.38", 88888);
	/*client.StartLoop(LOOP_RUN_TYPE::LOOP_RUN_NONBLOCK);*/
	client.StartLoop(LOOP_RUN_TYPE::LOOP_RUN_BLOCK);
	client.StopLoop();
	while (1)
	{
		SFSLEEP(100);
	}
	return 0;
}