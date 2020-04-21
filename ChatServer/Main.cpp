#include <signal.h>
#include "SePlatForm.h"
#include "JsonConfig.h"
#include "Chat.h"
#include "LogUtil.h"

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
	g_pConfig->Load(SERVER_CFG);
	g_pConfig->m_ServerConf = g_pConfig->m_Root["ChatServer"];

	INIT_SFLOG("ChatServer");

	Chat chat;
	chat.Init();
	chat.Start();

	while (bStopServer == false)
	{
		sf_sleep(500);
	}

	chat.Stop();
	STOP_SFLOG();
    return 0;
}
