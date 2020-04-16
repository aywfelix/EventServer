#include <signal.h>
#include "SePlatForm.h"
#include "Login.h"
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
		CLOG_ERR << "Login Server Stop !!!, signal=" << sig << CLOG_END;
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
	g_pConfig.reset(new JsonConfig());
	g_pConfig->Load("../Config/ServerConf.json");
	g_pConfig->m_ServerConf = g_pConfig->m_Root["LoginServer"];
	g_pConfig->m_RedisConf = g_pConfig->m_Root["Redis"];
	//mariadb config
	g_pConfig->m_dbConf = g_pConfig->m_Root["MariaDB"];

	INIT_SFLOG("LoginServer");
	Login login;
	login.Init();
	login.Start();

	while (bStopServer == false)
	{
		sf_sleep(500);
	}
	login.Stop();
	STOP_SFLOG();
	return 0;
}