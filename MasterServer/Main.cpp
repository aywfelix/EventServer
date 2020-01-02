
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
#ifdef _WIN32
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
#ifdef _WIN32
	signal(SIGBREAK, OnSignal);
#else
	signal(SIGPIPE, OnSignal);
#endif
}


int main()
{
	OnHookSignal();
	g_pJsonConfig.reset(new JsonConfig);
	g_pJsonConfig->Load("../Config/ServerConf.json");
	g_pJsonConfig->m_ServerConf = g_pJsonConfig->m_Root["MasterServer"];
	INIT_SFLOG(true);
	Master master;
	master.Init();
	master.Start();

	while (bStopServer == false)
	{
		SFSLEEP(500);
	}

	master.Stop();

    return 0;
}