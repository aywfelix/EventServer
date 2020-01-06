#include "DB.h"
#include "JsonConfig.h"
#include "Util.h"
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
#ifdef _WIN32
	signal(SIGBREAK, OnSignal);
#else
	signal(SIGPIPE, OnSignal);
#endif
}


int main()
{
	OnHookSignal();

	//server config
	g_pJsonConfig.reset(new JsonConfig());
	g_pJsonConfig->Load("../Config/ServerConf.json");
	g_pJsonConfig->m_ServerConf = g_pJsonConfig->m_Root["DBServer"];
	//mariadb config
	g_pJsonConfig->m_dbConf = g_pJsonConfig->m_Root["MariaDB"];

	DB db;
	db.Init();
	db.Start();

	while (bStopServer == false)
	{
		SFSLEEP(500);
	}

	db.Stop();

	return 0;
}