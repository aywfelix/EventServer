#pragma once

#include "SeFClientBase.h"

class ChatNodeClient : public SeFClientBase {
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();
	void OnGateRouteChat(const socket_t sock_fd, const int msg_id, const char* msg, const uint32_t msglen);
	void OnGameRouteChat(const socket_t sock_fd, const int msg_id, const char* msg, const uint32_t msglen);
	void SendToGate(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pMsg);
private:

};