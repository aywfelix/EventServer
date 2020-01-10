#pragma once

#include "SeFINet.h"
#include "NetServerBase.h"

class MasterNodeServer : public NetServerBase{
public:
    bool InitHelper();
    void AfterReportToServer(ServerDataPtr& pReportServerData);
    void SyncNodeDataToAll();
};