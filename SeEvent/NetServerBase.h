#pragma once

#include "SeFINet.h"


class NetServerBase
{
public:
	virtual bool Init();
	virtual bool InitHelper() = 0;
	virtual void Loop();
	virtual void OnClientSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet);
	virtual void OnClientConnected(const socket_t nFd);
	virtual void OnClientDisconnect(const socket_t nFd);
	SeFNet* GetNetModule() { return mpNetModule; }

	ServerDataPtr GetClientNodeData(int nServerId);

	virtual void OnReportToServer(const socket_t nFd, const int nMsgID, const char* msg, const uint32_t nLen);
	virtual void AfterReportToServer(ServerDataPtr& pReportServerData);

protected:
	SeFNet* mpNetModule;
	// serverid serverdata
	std::map<int, ServerDataPtr> mmClientNodes;
};

