#pragma once

#include <list>
#include <map>

#include "Se.h"
#include "SeFINet.h"

class SeFNetServer
{
public:
	void Excute();
	void RemoveReceiveCallBack(const int nMsgID);
	bool AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR& cb);
	bool AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb);
	bool AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb);
private:
	seEventLoop* pNet;

	// msgid  msghandle
	std::map<int, NET_RECEIVE_FUNCTOR_PTR> mReceiveCallBack;
	std::list<NET_RECEIVE_FUNCTOR_PTR> mReceiveCallBackList;
	std::list<NET_EVENT_FUNCTOR_PTR> mEventCallBackList;
};

