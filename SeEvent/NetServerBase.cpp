#include "NetServerBase.h"
#include "SeFNodeNet.pb.h"
#include "LogHelper.h"
#include "SeFNet.h"

using namespace SeFNetProto;

bool NetServerBase::Init()
{
	mpNetModule = new SeFNet();
	mpNetModule->AddEventCallBack(this, &NetServerBase::OnClientSocketEvent);
	mpNetModule->AddReceiveCallBack(REPORT_CLIENT_INFO_TO_SERVER, this, &NetServerBase::OnReportToServer);
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
	int nServerId = -1;
	for (auto it = mmClientNodes.begin(); it != mmClientNodes.end(); it++)
	{
		if (it->second->fd == nFd)
		{
			nServerId = it->second->ServerInfo->server_id();
			mmClientNodes.erase(nServerId);
			break;
		}
	}
}

ServerDataPtr NetServerBase::GetClientNodeData(int nServerId)
{
	auto it = mmClientNodes.find(nServerId);
	if (it == mmClientNodes.end())
	{
		return nullptr;
	}
	return it->second;
}

void NetServerBase::OnReportToServer(const socket_t nFd, const int nMsgID, const char* msg, const uint32_t nLen)
{
	ServerDataPtr pServerData = std::make_shared<ServerData>();
	SeFNetProto::ServerReport report;
	if (!SeNet::ReceivePB(nMsgID, msg, nLen, &report))
	{
		return;
	}
	Assert(report.server_id() != 0);
	pServerData->fd = nFd;
	pServerData->ServerInfo = std::make_shared<SeFNetProto::ServerReport>(report);
	mmClientNodes.emplace(report.server_id(), pServerData);
	for (auto& it : mmClientNodes)
	{
		LOG_INFO("clients(%d : %s)", it.first, it.second->ServerInfo->server_name().c_str());
	}
	AfterReportToServer(pServerData);  // 被子类重新接口
}
void NetServerBase::AfterReportToServer(ServerDataPtr& pReportServerData)
{

}

void NetServerBase::Loop()
{
	mpNetModule->Execute(LOOP_RUN_TYPE::LOOP_RUN_NONBLOCK);
}
