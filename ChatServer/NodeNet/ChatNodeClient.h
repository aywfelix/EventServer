#pragma once

#include "SeFClientBase.h"

class ChatNodeClient : public SeFClientBase {
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();
	void OnGateRouteChat(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);
	void OnGameRouteChat(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);
	void SendToGate(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* msg);
private:

};