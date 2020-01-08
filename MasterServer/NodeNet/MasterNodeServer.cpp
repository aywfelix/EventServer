#include "MasterNodeServer.h"
#include "JsonConfig.h"
#include "LogHelper.h"
#include "SeFNodeNet.pb.h"

using namespace SeFNetProto;

bool MasterNodeServer::InitHelper()
{
	//init server info
	if (!mpNetModule->InitNet(g_pJsonConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init MasterNodeServer failed");
		return false;
	}
	CLOG_INFO << "init MasterNodeServer ok" << CLOG_END;
	return true;
}

void MasterNodeServer::AfterReportToServer(ServerDataPtr& pReportServerData)
{
	EServerType nExcludeType = (EServerType)pReportServerData->ServerInfo->server_type();
	SyncNodeDataToAll(nExcludeType);  // 对应连接类型
	LOG_INFO("sync node server info : %s", pReportServerData->ServerInfo->server_name());
}

void MasterNodeServer::SyncNodeDataToAll(EServerType nType)
{
	ServerReportList report_list;
	//std::map<int, ServerDataPtr> mmClientNodes;
	for (auto it = mmClientNodes.begin(); it != mmClientNodes.end(); it++)
	{
		ServerReport* pReport = report_list.add_server_info();
		if (pReport == nullptr)
		{
			continue;
		}
		pReport->CopyFrom(*(it->second->ServerInfo));
	}
	if (report_list.server_info_size() > 0)
	{
		for (auto it = mmClientNodes.begin(); it != mmClientNodes.end(); it++)
		{
			mpNetModule->SendPBMsg(it->second->fd, MASTER_REPORT_SERVER_INFO_TO_SERVER, &report_list);
		}
	}
}
