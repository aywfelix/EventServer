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
	virtual void OnClientSocketEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet);
	virtual void OnClientConnected(const socket_t sock_fd);
	virtual void OnClientDisconnect(const socket_t sock_fd);
	SeFNet* GetNetModule() { return mNetServModule; }

	ServerDataPtr GetClientNodeData(int nServerId);

	virtual void OnReportToServer(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);
	virtual void AfterReportToServer(ServerDataPtr& pReportServerData);

	void SendByServType(ServerType type, const int msg_id, const char* msg, int len);
	void SendByServId(int nServerId, const int msg_id, const char* msg, int len);
	void SendPbByServType(ServerType type, const int msg_id, ::google::protobuf::Message* pb_msg);
	void SendPbByServId(int nServerId, const int msg_id, ::google::protobuf::Message* pb_msg);

	void SendByServTypeOne(ServerType type, const int msg_id, const char* msg, int len);
	void SendPbByServTypeOne(ServerType type, const int msg_id, ::google::protobuf::Message* pb_msg);
protected:
	SeFNet* mNetServModule;
	// serverid serverdata
	std::map<int, ServerDataPtr> mmServNodes;
};

