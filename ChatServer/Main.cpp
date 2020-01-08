#include "JsonConfig.h"
#include "Util.h"
#include "Chat.h"
#include "LogHelper.h"
#include "SePlatForm.h"
#include <signal.h>

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
		CLOG_ERR << "Master Server Stop !!!, signal=" << sig <<CLOG_END;
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
#ifdef SF_PLATFORM_LINUX
	SetResource();
#endif
    g_pJsonConfig.reset(new JsonConfig());
	g_pJsonConfig->Load("../Config/ServerConf.json");
	g_pJsonConfig->m_ServerConf = g_pJsonConfig->m_Root["ChatServer"];

	INIT_SFLOG("ChatServer");

	Chat chat;
	chat.Init();
	chat.Start();

	while (bStopServer == false)
	{
		SFSLEEP(500);
	}

	chat.Stop();

    return 0;
}
