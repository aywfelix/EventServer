#include "DB.h"
#include "JsonConfig.h"
#include "Util.h"
#include "LogUtil.h"
#include "SePlatForm.h"
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


int main()
{
	OnHookSignal();

	//server config
	g_pConfig.reset(new JsonConfig());
	g_pConfig->Load("../Config/ServerConf.json");
	g_pConfig->m_ServerConf = g_pConfig->m_Root["DBServer"];
	//mariadb config
	g_pConfig->m_dbConf = g_pConfig->m_Root["MariaDB"];
	INIT_SFLOG("DBServer");

	DB db;
	db.Init();
	db.Start();

	while (bStopServer == false)
	{
		sf_sleep(500);
	}

	db.Stop();

	return 0;
}