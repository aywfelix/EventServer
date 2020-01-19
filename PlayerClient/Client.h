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

	void OnSocketEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet);
	void OnMessage(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);

protected:
	SeFNetClient* m_pNetClientModule{ nullptr };
	std::vector<ServerType> m_connectType;

	int m_ServerId;
};