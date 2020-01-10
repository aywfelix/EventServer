#include <iostream>
#include "WorldNodeServer.h"
#include "JsonConfig.h"
#include "LogHelper.h"

bool WorldNodeServer::InitHelper()
{
	mNetServModule->AddReceiveCallBack(this, &WorldNodeServer::OtherMessage);

	//init server info
	if (!mNetServModule->InitNet(g_JsonConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init WorldServer failed");
		return false;
	}
	CLOG_INFO << "init WorldServer ok" << CLOG_END;
	return true;
}

void WorldNodeServer::OtherMessage(const socket_t nFd, const int nMsgID, const char* msg, const uint32_t nLen)
{

}

