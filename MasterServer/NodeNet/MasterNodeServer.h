#pragma once

#include "SeFINet.h"
#include "SeFServerBase.h"

class MasterNodeServer : public SeFServerBase{
public:
    bool InitHelper();
    void AfterReportToServer(ServerDataPtr& pReportServerData);
    void SyncNodeDataToAll();
};