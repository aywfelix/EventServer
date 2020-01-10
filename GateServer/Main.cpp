#include <signal.h>
#include "Gate.h"
#include "SePlatForm.h"
#include "JsonConfig.h"
#include "LogHelper.h"


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
		LOG_ERR("Master Server Stop !!!, signal=%d", sig);
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

	//init config
    g_JsonConfig.reset(new JsonConfig);
	g_JsonConfig->Load("../Config/ServerConf.json");
	g_JsonConfig->m_ServerConf = g_JsonConfig->m_Root["GateServer"];
	INIT_SFLOG("GateServer");

	Gate gate;
	gate.Init();
	gate.Start();

	while (bStopServer == false)
	{
		SFSLEEP(500);
	}

	gate.Stop();
	STOP_SFLOG();
    return 0;
}

