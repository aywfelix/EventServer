#include "JsonConfig.h"
#include "Util.h"
#include "Client.h"
#include "LogUtil.h"
#include "SePlatForm.h"
#include "Session.h"

int main()
{
	INIT_SFLOG("PlayerClient");
	CLOG_INFO << "client connect to gate ok" << CLOG_END;
	//init config
	g_pConfig.reset(new JsonConfig);
	g_pConfig->Load(SERVER_CFG);
	g_pConfig->m_ServerConf = g_pConfig->m_Root["GateServer"];

	g_pSessionPool = std::make_unique<SessionPool>();

	Client client;
	client.Init();
	while (true)
	{
		client.Loop();
		sf_sleep(500);
	}

	STOP_SFLOG();
	return 0;
}