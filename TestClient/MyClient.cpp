#include "MyClient.h"
#include "SeSelect.h"
#include "LogHelper.h"
#include "Session.h"

int main()
{
	g_pSessionPool = std::make_unique<SessionPool>();
	INIT_SFLOG(true, true);
	seEventLoop client;
	client.Init();
	/*client.InitClient("192.168.60.49", 88888);*/
	client.InitClient("192.168.43.38", 88888);
	client.StartLoop();
	while (1)
	{
		SFSLEEP(100);
	}
	return 0;
}