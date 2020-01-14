#pragma once

#include "SeFINet.h"

class SeFNet;

// 服务器保存连接的客户端信息 客户端上报的信息
struct ServerData
{
	std::shared_ptr<ServerReport> ServerInfo;
	socket_t fd;
};

using ServerDataPtr = std::shared_ptr<ServerData>;

class SeFServerBase
{
public:
	virtual bool Init();
	virtual bool InitHelper() = 0;
	virtual void Loop();
	virtual void OnClientSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet);
	virtual void OnClientConnected(const socket_t nFd);
	virtual void OnClientDisconnect(const socket_t nFd);
	SeFNet* GetNetModule() { return mNetServModule; }

	ServerDataPtr GetClientNodeData(int nServerId);

	virtual void OnReportToServer(const socket_t nFd, const int nMsgID, const char* msg, const uint32_t nLen);
	virtual void AfterReportToServer(ServerDataPtr& pReportServerData);

protected:
	SeFNet* mNetServModule;
	// serverid serverdata
	std::map<int, ServerDataPtr> mmServNodes;
};

