#include "World.h"
#include "SePlatForm.h"
#include "JsonConfig.h"
#include "LogUtil.h"
#include <signal.h>

bool bStopServer = false;

void OnSignal(int sig)
{
	switch (sig)
	{
	case SIGINT:
	case SIGTERM:
#ifdef SF_PLATFORM_WIN
	case SIGBREAK:
#else
	case SIGPIPE:
#endif
	{
		CLOG_ERR << "World Server Stop !!!, signal=" << sig <<CLOG_END;
		bStopServer = true;
		break;
	}
	default:
		break;
	}
}

void OnHookSignal()
{
	signal(SIGINT, OnSignal);
	signal(SIGTERM, OnSignal);
#ifdef SF_PLATFORM_WIN
	signal(SIGBREAK, OnSignal);
#else
	signal(SIGPIPE, OnSignal);
#endif
}


int main()
{
	OnHookSignal();
#ifdef SF_PLATFORM_LINUX
	SetResource();
#endif
	g_pConfig.reset(new JsonConfig());
	g_pConfig->Load("../Config/ServerConf.json");
	g_pConfig->m_ServerConf = g_pConfig->m_Root["WorldServer"];
	INIT_SFLOG("WorldServer");

	World world;
	world.Init();
	world.Start();

	while (bStopServer == false)
	{
		sf_sleep(500);
	}
	world.Stop();
	STOP_SFLOG();
	return 0;
}