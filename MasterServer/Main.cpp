
#include <signal.h>
#include "Master.h"
#include "JsonConfig.h"
#include "LogHelper.h"
#include "SePlatForm.h"

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
		LOG_INFO("Master Server Stop !!!, signal=%d", sig);
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
	g_JsonConfig.reset(new JsonConfig);
	g_JsonConfig->Load("../Config/ServerConf.json");
	g_JsonConfig->m_ServerConf = g_JsonConfig->m_Root["MasterServer"];
	INIT_SFLOG("MasterServer");
	Master master;
	master.Init();
	master.Start();

	while (bStopServer == false)
	{
		sf_sleep(500);
	}

	master.Stop();
	STOP_SFLOG();
    return 0;
}