#include "MyClient.h"
#include "SeSelect.h"
#include "LogHelper.h"
#include "Session.h"

int main()
{
	g_pSessionPool = std::make_unique<SessionPool>();
	INIT_SFLOG(true);
	SeNet client;

	client.InitClient("192.168.60.49", 88888);
	client.SendProtoMsg(111, "1234567890", 10);
	/*client.SendMsg("1111111111", 10);*/
	
	//client.InitClient("192.168.43.38", 88888);
	client.StartLoop(LOOP_RUN_TYPE::LOOP_RUN_NONBLOCK);
	client.StopLoop();
	while (1)
	{
		SFSLEEP(100);
	}
	return 0;
}