#include "SeFNet.h"

void SeFNet::Excute(LOOP_RUN_TYPE run)
{
	mNet->StartLoop(run);
}

void SeFNet::RemoveReceiveCallBack(const int nMsgID)
{
	auto it = mReceiveCallBack.find(nMsgID);
	if (it != mReceiveCallBack.end())
	{
		mReceiveCallBack.erase(it);
	}
}

void SeFNet::AddReceiveCallBack(const int nMsgID, const NET_RECEIVE_FUNCTOR_PTR& cb)
{
	auto it = mReceiveCallBack.find(nMsgID);
	if (it != mReceiveCallBack.end())
	{
		return;
	}
	mReceiveCallBack.emplace(nMsgID, cb);
}

void SeFNet::AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb)
{
	mReceiveCallBackList.emplace_back(cb);
}

void SeFNet::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb)
{
	mEventCallBackList.emplace_back(cb);
}

bool SeFNet::InitNet(const char* ip, UINT32 port)
{
	mNet = new SeNet(this, &SeFNet::OnReceiveNetPack, &SeFNet::OnSocketNetEvent);
	return mNet->InitClient(ip, port);
}

bool SeFNet::InitNet(UINT32 port)
{
	mNet = new SeNet(this, &SeFNet::OnReceiveNetPack, &SeFNet::OnSocketNetEvent);
	return mNet->InitServer(port);
}

void SeFNet::OnReceiveNetPack(const socket_t nFd, const int nMsgId, const char* pMsg, const uint32_t nLen)
{
	auto it = mReceiveCallBack.find(nMsgId);
	if (it != mReceiveCallBack.end())
	{
		it->second->operator()(nFd, nMsgId, pMsg, nLen);
	}
	else
	{
		for (auto& it : mReceiveCallBackList)
		{
			it->operator()(nFd, nMsgId, pMsg, nLen);
		}
	}
}

void SeFNet::OnSocketNetEvent(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	for (auto& it : mEventCallBackList)
	{
		it->operator()(nFd, nEvent, pNet);
	}
}