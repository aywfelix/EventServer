#pragma once

#include <list>
#include <map>

#include "Se.h"
#include "SeFINet.h"

class SeFNetServer
{
public:
	void Excute();
private:
	seEventLoop* pNet;

	// msgid  msghandle
	std::map<int, NET_RECEIVE_FUNCTOR_PTR> mReceiveCallBack;
	std::list<NET_RECEIVE_FUNCTOR_PTR> mReceiveCallBackList;
	std::list<NET_EVENT_FUNCTOR_PTR> mEventCallBackList;
};

