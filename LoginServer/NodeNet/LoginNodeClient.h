#pragma once

#include "SeFClientBase.h"


class LoginNodeClient : public SeFClientBase {
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();

	void OnGateRouteLogin(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);
	void OnDBRouteLogin(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);
	void SendToGate(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pb_msg);
	void SendToDB(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pb_msg);
	void SendToWorld(const int& serverid, uint64_t playerId, const int msg_id, ::google::protobuf::Message* pb_msg);

private:
};