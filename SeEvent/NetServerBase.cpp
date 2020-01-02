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
		}
	}
	mmClientNodes.erase(nServerId);
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
	if (!mpNetModule->ReceivePB(nMsgID, msg, nLen, &report))
	{
		return;
	}

	pServerData->fd = nFd;
	*(pServerData->ServerInfo) = report;
	mmClientNodes.erase(report.server_id());
	mmClientNodes.emplace(report.server_id(), pServerData);
	AfterReportToServer(pServerData);
	LOG_INFO("report server info from %d", report.server_id());
}
void NetServerBase::AfterReportToServer(ServerDataPtr pReportServerData)
{

}

void NetServerBase::Loop()
{
	mpNetModule->Excute(LOOP_RUN_TYPE::LOOP_RUN_NONBLOCK);
}
