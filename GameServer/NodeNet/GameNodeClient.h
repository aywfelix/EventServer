#pragma once

#include "SeFClientBase.h"


class GameNodeClient : public SeFClientBase {
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();
    void OnSocketEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet);
    
private:
};