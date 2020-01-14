#include "JsonConfig.h"
#include "Util.h"
#include "Client.h"
#include "LogHelper.h"
#include "SePlatForm.h"
#include "Session.h"

int main()
{
	INIT_SFLOG("PlayerClient");
	CLOG_INFO << "client connect to gate ok" << CLOG_END;
	//init config
	g_JsonConfig.reset(new JsonConfig);
	g_JsonConfig->Load("../Config/ServerConf.json");
	g_JsonConfig->m_ServerConf = g_JsonConfig->m_Root["GateServer"];

	g_pSessionPool = std::make_unique<SessionPool>();

	Client client;
	client.Init();
	while (true)
	{
		client.Loop();
		SFSLEEP(500);
	}

	STOP_SFLOG();
	return 0;
}