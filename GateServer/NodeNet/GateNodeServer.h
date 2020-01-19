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
    bool SendPackToLogin(socket_t client_fd, const int msg_id, const char *msg, size_t msg_len, int nGameID, const std::string &ip);
    bool SendPackToScene(const int msg_id, google::protobuf::Message *msg, int nServerID);
    bool SentPackToChat(const int msg_id, google::protobuf::Message *msg);
    bool SentPackToWorld(const int msg_id, google::protobuf::Message *msg);

    //back msg to client from other server
    void OnLoginRouteBack(socket_t sock_fd, const int msg_id, const char *msg, const size_t msg_len);
    void OnChatRouteBack(socket_t sock_fd, const int msg_id, const char *msg, const size_t msg_len);
    void OnWorldRouteBack(socket_t sock_fd, const int msg_id, const char *msg, const size_t msg_len);
};