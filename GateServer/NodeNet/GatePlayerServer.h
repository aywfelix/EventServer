#pragma once

#include "SePlatForm.h"
#include "SeNet.h"
#include "SeFINet.h"
#include <string>

class SeFNet;

class GatePlayerServer
{
public:
	bool Init();
	void OnSocketClientEvent(const socket_t sock_fd, const SE_NET_EVENT eEvent, SeNet* pNet);
	void OnClientConnected(const socket_t sock_fd);
	void OnClientDisconnect(const socket_t sock_fd);

	void OnOtherMessage(const socket_t sock_fd, const int msg_id, const char* msg, const uint32_t msg_len);
	void SentToClient(const int nMsgID, const std::string& msg, const socket_t sock_fd);
	void SentToClient(const int nMsgID, google::protobuf::Message* msg, const socket_t sock_fd);
	void SentToAllClient(const int nMsgID, const std::string& msg);

	void KickPlayer(const socket_t sock_fd);
	void KickPlayerAllPlayer();

	int GetModuleID(const int msg_id);

	void Loop();
	SeFNet* GetNetModule()
	{
		return m_pNetModule;
	}
	void SetClientServerNode(socket_t client_fd, ServerDataPtr& ptr);

protected:

	int timestamp;
	long msg_recv_bytes;
	long msg_sent_bytes;
	std::string m_infoStr;
	SeFNet* m_pNetModule;
};