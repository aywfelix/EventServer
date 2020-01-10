#include "NetServerBase.h"
#include "SeFNodeNet.pb.h"
#include "LogHelper.h"
#include "SeFNet.h"

bool NetServerBase::Init()
{
	mNetServModule = new SeFNet();
	mNetServModule->AddEventCallBack(this, &NetServerBase::OnClientSocketEvent);
	mNetServModule->AddReceiveCallBack(REPORT_CLIENT_INFO_TO_SERVER, this, &NetServerBase::OnReportToServer);
	return true;
}

void NetServerBase::OnClientSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	switch (nEvent)
	{
	case SE_NET_EVENT_CONNECTED:
		OnClientConnected(nFd);
		break;
	case SE_NET_EVENT_EOF:
	case SE_NET_EVENT_ERROR:
	case SE_NET_EVENT_TIMEOUT:
	default:
		OnClientDisconnect(nFd);
		break;
	}
}
void NetServerBase::OnClientConnected(const socket_t nFd)
{
	LOG_INFO("client %d connected ok", nFd);
}
void NetServerBase::OnClientDisconnect(const socket_t nFd)
{
	LOG_INFO("client %d disconnected", nFd);
	for (auto it = mmServNodes.begin(); it != mmServNodes.end(); it++)
	{
		if (it->second->fd == nFd)
		{
			auto nServerId = it->second->ServerInfo->server_id();
			mmServNodes.erase(nServerId);
			break;
		}
	}
}

ServerDataPtr NetServerBase::GetClientNodeData(int nServerId)
{
	auto it = mmServNodes.find(nServerId);
	if (it == mmServNodes.end())
	{
		return nullptr;
	}
	return it->second;
}

void NetServerBase::OnReportToServer(const socket_t nFd, const int nMsgID, const char* msg, const uint32_t nLen)
{
	ServerDataPtr pServerData = std::make_shared<ServerData>();
	ServerReport report;
	if (!SeNet::ReceivePB(nMsgID, msg, nLen, &report))
	{
		return;
	}
	pServerData->fd = nFd;
	pServerData->ServerInfo = std::make_shared<ServerReport>(report);
	mmServNodes.emplace(report.server_id(), pServerData);
	for (auto& it : mmServNodes)
	{
		LOG_INFO("clients(%d : %s)", it.first, it.second->ServerInfo->server_name().c_str());
	}
	AfterReportToServer(pServerData);  // 被子类重新接口
}
void NetServerBase::AfterReportToServer(ServerDataPtr& pReportServerData)
{}

void NetServerBase::Loop()
{
	mNetServModule->Execute(LOOP_RUN_TYPE::LOOP_RUN_NONBLOCK);
}
