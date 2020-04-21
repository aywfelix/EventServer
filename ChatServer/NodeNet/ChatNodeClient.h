#pragma once

#include "SeFClientBase.h"

class ChatNodeClient : public SeFClientBase {
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();
	void OnGateRouteChat(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);
	void SendToGate(const int& server_id, uint64_t player_id, const int msg_id, ::google::protobuf::Message* pb_msg);
private:

};