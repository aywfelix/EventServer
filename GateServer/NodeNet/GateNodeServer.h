#pragma once

#include "SeFServerBase.h"

//as server
class GateNodeServer : public SeFServerBase
{
public:
    virtual bool InitHelper();
    virtual void OnClientDisconnect(const socket_t nFd) {}
    void OtherMessage(const socket_t nFd, const int msgid, const char *msg, const uint32_t nLen);

    //send msg to other server
    bool SendPackToLogin(socket_t client_fd, const int msgid, const char *msg, uint32_t nLen, int nGameID, const std::string &ip);
    bool SendPackToScene(const int msgid, google::protobuf::Message &xData, int nServerID);
    bool SentPackToChat(const int msgid, google::protobuf::Message &xData);
    bool SentPackToWorld(const int msgid, google::protobuf::Message &xData);

    //back msg to client from other server
    void OnLoginRouteBack(socket_t nFd, const int msgid, const char *msg, const uint32_t nLen);
    void OnChatRouteBack(socket_t nFd, const int msgid, const char *msg, const uint32_t nLen);
    void OnWorldRouteBack(socket_t nFd, const int msgid, const char *msg, const uint32_t nLen);
};