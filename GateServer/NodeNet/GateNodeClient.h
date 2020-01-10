#pragma once

#include "NetClientBase.h"

class SeNet;
class GateNodeClient : public NetClientBase{
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();
    void OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet);
   
private:
};