#include <signal.h>
#include "SePlatForm.h"
#include "Game.h"
#include "JsonConfig.h"
#include "Util.h"
#include "LogUtil.h"

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
		CLOG_ERR << "Master Server Stop !!!, signal=" << sig << CLOG_END;
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


int main(int argc, char** argv)
{
	OnHookSignal();
#ifdef SF_PLATFORM_LINUX
	SetResource();
#endif
	std::string gameserver = "GameServer" + std::string(argv[1]);
	g_pConfig.reset(new JsonConfig());
	g_pConfig->Load("../Config/ServerConf.json");
	g_pConfig->m_ServerConf = g_pConfig->m_Root[gameserver];
	g_pConfig->m_RedisConf = g_pConfig->m_Root["Redis"];
	INIT_SFLOG(gameserver);

	Game game;
	game.Init();
	game.Start();

	while (bStopServer == false)
	{
		sf_sleep(500);
	}
	game.Stop();
	STOP_SFLOG();
	return 0;
}