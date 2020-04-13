#pragma once

#include "SeFClientBase.h"


class GameNodeClient : public SeFClientBase {
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();
    void OnSocketEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet);
	void OnGateRouteGame(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);
	void OnWorldRouteGame(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);
	
	void SendToGate(const int& server_id, uint64_t player_id, const int msg_id, ::google::protobuf::Message* pb_msg);
	void SendToWorld(const int& server_id, uint64_t player_id, const int msg_id, ::google::protobuf::Message* pb_msg);
private:
};