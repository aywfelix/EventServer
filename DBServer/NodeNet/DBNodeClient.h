#pragma once
#include "SeFClientBase.h"

class DBNodeClient : public SeFClientBase{
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();
    void OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet);
   
private:
};