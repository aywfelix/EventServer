#pragma once

#include <functional>
#include <memory>
#include "SePlatForm.h"
#include "Se.h"


class SFINet;

typedef std::function<void(const socket_t nSock, const int nMsgID, const char* msg, const uint32_t nLen)> NET_RECEIVE_FUNCTOR;
typedef std::shared_ptr<NET_RECEIVE_FUNCTOR> NET_RECEIVE_FUNCTOR_PTR;

typedef std::function<void(const socket_t nSock, const SF_NET_EVENT nEvent, SFINet * pNet)> NET_EVENT_FUNCTOR;
typedef std::shared_ptr<NET_EVENT_FUNCTOR> NET_EVENT_FUNCTOR_PTR;

class NFINet
{
public:
	virtual ~NFINet()
	{}

	//need to call this function every frame to drive network library
	virtual bool Execute() = 0;

	//as client
	virtual void Initialization(const char* strIP, const unsigned short nPort) = 0;

	//as server
	virtual int Initialization(const unsigned short nPort, const unsigned int nMaxClient) = 0;
	virtual int Initialization(const char* strIP, const unsigned short nPort, const unsigned int nMaxClient) = 0;

	virtual int ExpandBufferSize(const unsigned int size) = 0;

	virtual bool Final() = 0;

	//send a message with out msg-head[auto add msg-head in this function]
	virtual bool SendMsgWithOutHead(const int16_t nMsgID, const char* msg, const size_t nLen, const NFSOCK nSockIndex = 0) = 0;

	//send a message with out msg-head[need to add msg-head for this message by youself]
	virtual bool SendMsg(const char* msg, const size_t nLen, const socket_t nSock) = 0;

	//send a message to all client[need to add msg-head for this message by youself]
	virtual bool SendMsgToAllClient(const char* msg, const size_t nLen) = 0;

	//send a message with out msg-head to all client[auto add msg-head in this function]
	virtual bool SendMsgToAllClientWithOutHead(const int16_t nMsgID, const char* msg, const size_t nLen) = 0;

	virtual bool CloseNetObject(const socket_t nSock) = 0;

	virtual bool IsServer() = 0;

	virtual bool Log(int severity, const char* msg) = 0;

	virtual bool KickAll() = 0;
};
