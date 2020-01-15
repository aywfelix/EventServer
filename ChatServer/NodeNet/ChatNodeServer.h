#pragma once
#include "SeFServerBase.h"
#include "SeFINet.h"

//as server
class ChatNodeServer : public SeFServerBase
{
public:
	virtual bool InitHelper();
	virtual void OnClientDisconnect(const socket_t nFd) {}

	//send msg to other server
	bool SendPackToGame(int nServerID, const int msgid, google::protobuf::Message* xData);
	bool SendPackToGame(const int msgid, google::protobuf::Message* xData);

	//back msg to client from other server
	void OnGameRouteBack(socket_t nFd, const int msgid, const char* msg, const uint32_t nLen);
};