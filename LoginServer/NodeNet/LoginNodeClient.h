#pragma once

#include "SeFClientBase.h"


class LoginNodeClient : public SeFClientBase {
public:
    void InitHelper();
    void SetReportInfo();
    void AddConnectServer();
private:
};