#include <iostream>
#include "DBNodeServer.h"
#include "JsonConfig.h"
#include "LogUtil.h"
#include "SeFNet.h"

bool DBNodeServer::InitHelper()
{
	mNetServModule->AddReceiveCallBack(this, &DBNodeServer::OtherMessage);

	//init server info
	if (!mNetServModule->InitNet(g_JsonConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init DBNodeServer failed");
		return false;
	}
	CLOG_INFO << "init DBNodeServer ok" << CLOG_END;
	return true;
}

void DBNodeServer::OtherMessage(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{

}

