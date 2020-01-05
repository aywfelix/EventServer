#pragma once

#include "NetClientBase.h"

class ChatNodeClient : public NetClientBase {
public:
    void InitHelper();
    void SetServerInfoReport();
    void AddConnectServer();
	void OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet);
    
	void OnGateRouteChat(const socket_t nFd, const int msgid, const char* msg, const uint32_t msglen);
	void SendToGate(const int GameID, uint64_t playerId, uint32_t msg_id, ::google::protobuf::Message* pMsg);
private:
	//void AddConnectMaster();
};