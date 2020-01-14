#pragma once

#include "SeFClientBase.h"
#include "SeNet.h"
#include <vector>


class Client
{
public:
	bool Init();
	void ConnectServer();
	bool Loop();

	void OnSocketEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet);
	void OnMessage(const socket_t nFd, const int msgid, const char* msg, const uint32_t msglen);

protected:
	SeFNetClient* m_pNetClientModule{ nullptr };
	std::vector<EServerType> m_ConnectType;

	int m_ServerId;
};