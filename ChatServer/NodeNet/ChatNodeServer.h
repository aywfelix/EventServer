#pragma once
#include "SeFServerBase.h"
#include "SeFINet.h"

//as server
class ChatNodeServer : public SeFServerBase
{
public:
	virtual bool InitHelper();
	virtual void OnClientDisconnect(const socket_t sock_fd) {}

	//send msg to other server
	bool SendPackToGame(int server_id, const int msg_id, google::protobuf::Message* msg);
	bool SendPackToGame(const int msg_id, google::protobuf::Message* msg);

	//back msg to client from other server
	void OnGameRouteBack(socket_t sock_fd, const int msg_id, const char* msg, const uint32_t msg_len);
};