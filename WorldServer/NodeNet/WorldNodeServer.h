#pragma once

#include "NetServerBase.h"

class WorldNodeServer : public NetServerBase{
public:
    virtual bool InitHelper();
    virtual void OnClientDisconnect(const socket_t nFd) {}
    void OtherMessage(const socket_t nFd, const int nMsgID, const char *msg, const uint32_t nLen);

};