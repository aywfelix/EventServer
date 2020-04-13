#pragma once

#include "SeFServerBase.h"

//as server
class GateNodeServer : public SeFServerBase
{
public:
    virtual bool InitHelper();
    virtual void OnClientDisconnect(const socket_t sock_fd) {}
    void OtherMessage(const socket_t sock_fd, const int msg_id, const char *msg, const size_t msg_len);

    //send msg to other server
    bool SendToLogin(const int msg_id, google::protobuf::Message* pb_msg);
    bool BroadcastToGame(const int msg_id, google::protobuf::Message * pb_msg);
	bool SendToGame(const int server_id, const int msg_id, google::protobuf::Message* pb_msg);
    bool SendToChat(const int msg_id, google::protobuf::Message * pb_msg);
    bool SendToWorld(const int msg_id, google::protobuf::Message * pb_msg);

    //back msg to client from other server
    void OnLoginRouteGate(socket_t sock_fd, const int msg_id, const char *msg, const size_t msg_len);
    void OnChatRouteGate(socket_t sock_fd, const int msg_id, const char *msg, const size_t msg_len);
    void OnWorldRouteGate(socket_t sock_fd, const int msg_id, const char *msg, const size_t msg_len);
};