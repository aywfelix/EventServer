#pragma once
#include "SeFClientBase.h"

class DBNodeClient : public SeFClientBase{
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();
private:
};