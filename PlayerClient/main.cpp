#include "JsonConfig.h"
#include "Util.h"
#include "Client.h"
#include "LogHelper.h"
#include "SePlatForm.h"

int main()
{
	INIT_SFLOG("PlayerClient");
	CLOG_INFO << "client connect to gate ok" << CLOG_END;
	//init config
	g_pJsonConfig.reset(new JsonConfig);
	g_pJsonConfig->Load("../Config/ServerConf.json");
	g_pJsonConfig->m_ServerConf = g_pJsonConfig->m_Root["GateServer"];

	Client client;
	client.Init();
	while (true)
	{
		client.Loop();
		SFSLEEP(500);
	}
	return 0;
}