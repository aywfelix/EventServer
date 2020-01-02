#pragma once
#include "NFNetPlugin/NFNetModule.h"
#include "NFCore/NFMapEx.hpp"

class GatePlayerServer
{
public:
	bool Init();
	void OnSocketClientEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
	void OnClientConnected(const NFSOCK nSockIndex);
	void OnClientDisconnect(const NFSOCK nSockIndex);

	void OnOtherMessage(const NFSOCK nSockIndex, const int msgid, const char* msg, const uint32_t nLen);
	void SentToClient(const int nMsgID, const std::string& msg, const NFSOCK nSockIndex);
	void SentToClient(int nMsgID, const google::protobuf::Message& xData, NFSOCK nSockIndex);
	void SentToAllClient(const int nMsgID, const std::string& msg);

	void KickPlayer(const NFSOCK nSockIndex);
	void KickPlayerAllPlayer();

	int GetModuleID(const int msgid);

	bool Loop();
	NFNetModule* GetNetModule()
	{
		return m_pNetModule;
	}
	void SetClientServerNode(NFSOCK client_fd, NF_SHARE_PTR<ServerData> ptr);

private:


protected:

	int timestamp;
	long msg_recv_bytes;
	long msg_sent_bytes;
	std::string m_infoStr;

	NFNetModule* m_pNetModule;
	//NFMapEx<NFSOCK, ServerData> m_mClientNodes;
};