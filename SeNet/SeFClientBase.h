#pragma once

#include "SeFINet.h"
#include "SeFNodeNet.pb.h"
#include "SeFNet.h"

class SeFNetClient;
class SeNet;

class SeFClientBase
{
public:
	virtual bool Init();
	virtual void InitHelper() = 0;
	virtual void SetReportInfo() = 0;
	virtual void AddConnectServer() = 0;
	virtual void OnMasterMessage(const socket_t sock_fd, const int msg_id, const char* msg, const uint32_t msg_len);
	virtual void Loop();

	virtual void OnSocketNodeEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet);

	ConnectDataPtr GetServerNetInfo(const int& nServerID);
	ConnectDataPtr GetServerNetInfo(const SeNet* pNet);
	ConnectDataPtr GetServerNetInfo(const socket_t& sock_fd);

	virtual ServerType GetServerType()
	{
		return (ServerType)mServerInfo.server_type();
	}
	virtual void AddConnectMaster();

protected:
	SeFNetClient* mNetCliModule{ nullptr };
	ServerReport mServerInfo;  // 上报master本服信息
	std::vector<ServerType> mConnectType;
};

