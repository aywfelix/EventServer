#include <iostream>
#include "DBNodeServer.h"
#include "JsonConfig.h"
#include "LogHelper.h"

bool DBNodeServer::InitHelper()
{
	mpNetModule->AddReceiveCallBack(this, &DBNodeServer::OtherMessage);

	//init server info
	if (!mpNetModule->InitNet(g_pJsonConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init DBNodeServer failed");
		return false;
	}
	CLOG_INFO << "init DBNodeServer ok" << CLOG_END;
	return true;
}

void DBNodeServer::OtherMessage(const socket_t nFd, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

