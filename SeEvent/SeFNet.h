#pragma once

#include <list>
#include <map>

#include "Se.h"

class SeFNet
{
public:
	void Excute();

	// init client
	bool InitNet(const char* ip, UINT32 port);
	// init server 
	bool InitNet(UINT32 port);


	// add callback
	template<typename BaseType>
	void AddReceiveCallBack(const int nMsgID, BaseType* pBase, void (BaseType::* HandleReceive)(const socket_t, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(HandleReceive, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr = std::make_shared<NET_RECEIVE_FUNCTOR>(functor);
		AddReceiveCallBack(nMsgID, functorPtr);
	}

	template<typename BaseType>
	void AddReceiveCallBack(BaseType* pBase, void (BaseType::* HandleReceive)(const socket_t, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(HandleReceive, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr = std::make_shared<NET_RECEIVE_FUNCTOR>(functor);
		AddReceiveCallBack(functorPtr);
	}

	template<typename BaseType>
	void AddEventCallBack(BaseType* pBase, void (BaseType::* handler)(const socket_t, const SE_NET_EVENT, SeNet* pNet))
	{
		NET_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NET_EVENT_FUNCTOR_PTR functorPtr = std::make_shared<NET_EVENT_FUNCTOR>(functor);
		AddEventCallBack(functorPtr);
	}
	void AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR& cb);
	void AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb);
	void AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb);
	void RemoveReceiveCallBack(const int nMsgID);
private:

	void OnReceiveNetPack(const socket_t nFd, const int nMsgId, const char* pMsg, const uint32_t nLen);
	void OnSocketNetEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet);
private:
	SeNet* mNet;
	// msgid  msghandle
	std::map<int, NET_RECEIVE_FUNCTOR_PTR> mReceiveCallBack;
	std::list<NET_RECEIVE_FUNCTOR_PTR> mReceiveCallBackList;
	std::list<NET_EVENT_FUNCTOR_PTR> mEventCallBackList;
};

