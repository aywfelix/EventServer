#include "MasterNodeServer.h"
#include "JsonConfig.h"
#include "LogUtil.h"
#include "SeFNodeNet.pb.h"
#include "SeFNet.h"

bool MasterNodeServer::InitHelper()
{
	//init server info
	if (!mNetServModule->InitNet(g_JsonConfig->m_ServerConf["NodePort"].asUInt()))
	{
		LOG_ERR("init MasterNodeServer failed");
		return false;
	}
	CLOG_INFO << "init MasterNodeServer ok" << CLOG_END;
	return true;
}

void MasterNodeServer::AfterReportToServer(ServerDataPtr& pReportServerData)
{
	SyncNodeDataToAll();  // 对应连接类型
}

void MasterNodeServer::SyncNodeDataToAll()
{
	ServerReportList report_list;
	//std::map<int, ServerDataPtr> mmServNodes;
	for (auto it = mmServNodes.begin(); it != mmServNodes.end(); it++)
	{
		ServerReport* pReport = report_list.add_server_info();
		if (pReport == nullptr) continue;
		pReport->CopyFrom(*(it->second->ServerInfo));
	}
	if (report_list.server_info_size() > 0)
	{
		for (auto it = mmServNodes.begin(); it != mmServNodes.end(); it++)
		{
			mNetServModule->SendPbMsg(it->second->fd, MASTER_REPORT_SERVER_INFO_TO_SERVER, &report_list);
		}

		LOG_INFO("==============================================");
		for (int i = 0; i < report_list.server_info_size(); i++)
		{
			ServerReport pReport = report_list.server_info(i);
			LOG_INFO("(%d : %s)", pReport.server_id(), pReport.server_name().c_str());
		}
		LOG_INFO("==============================================");
	}
}
