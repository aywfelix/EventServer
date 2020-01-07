#pragma once

#include "NetClientBase.h"


class GameNodeClient : public NetClientBase {
public:
    void InitHelper();
    void SetServerInfoReport();
    void AddConnectServer();
    void OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet);
    
private:
};