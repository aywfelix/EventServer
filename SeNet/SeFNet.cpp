#include "SeFNet.h"
#include "SeNet.h"

void SeFNet::Execute(LOOP_RUN_TYPE run)
{
	mNet->StartLoop(run);
}

void SeFNet::RemoveReceiveCallBack(const int msg_id)
{
	auto it = mReceiveCallBack.find(msg_id);
	if (it != mReceiveCallBack.end())
	{
		mReceiveCallBack.erase(it);
	}
}

void SeFNet::AddReceiveCallBack(const int msg_id, const NET_RECEIVE_FUNCTOR_PTR& cb)
{
	auto it = mReceiveCallBack.find(msg_id);
	if (it != mReceiveCallBack.end())
	{
		return;
	}
	mReceiveCallBack.emplace(msg_id, cb);
}

void SeFNet::AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb)
{
	mReceiveCallBackList.emplace_back(cb);
}

void SeFNet::AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb)
{
	mEventCallBackList.emplace_back(cb);
}

bool SeFNet::InitNet(const char* ip, uint32_t port)
{
	mNet = new SeNet(this, &SeFNet::OnReceiveNetPack, &SeFNet::OnSocketNetEvent);
	return mNet->InitClient(ip, port);
}

bool SeFNet::InitNet(uint32_t port)
{
	mNet = new SeNet(this, &SeFNet::OnReceiveNetPack, &SeFNet::OnSocketNetEvent);
	return mNet->InitServer(port);
}

void SeFNet::OnReceiveNetPack(const socket_t sock_fd, const int nMsgId, const char* pMsg, const uint32_t msg_len)
{
	auto it = mReceiveCallBack.find(nMsgId);
	if (it != mReceiveCallBack.end())
	{
		it->second->operator()(sock_fd, nMsgId, pMsg, msg_len);
	}
	else
	{
		for (auto& it : mReceiveCallBackList)
		{
			it->operator()(sock_fd, nMsgId, pMsg, msg_len);
		}
	}
}

void SeFNet::OnSocketNetEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	for (auto& it : mEventCallBackList)
	{
		it->operator()(sock_fd, nEvent, pNet);
	}
}

void SeFNet::SendMsg(const socket_t fd, const int msg_id, const char* msg, int len)
{
	mNet->SendProtoMsg(fd, msg_id, msg, len);
}
void SeFNet::SendMsg(std::vector<socket_t>& fdlist, const int msg_id, const char* msg, int len)
{
	mNet->SendProtoMsg(fdlist, msg_id, msg, len);
}
void SeFNet::SendToAll(const int msg_id, const char* msg, int len)
{
	mNet->SendProtoMsg(msg_id, msg, len);
}

void SeFNet::SendPbMsg(const socket_t fd, const int msg_id, ::google::protobuf::Message* pMsg)
{
	std::string strMsg = pMsg->SerializeAsString();
	if (strMsg.empty()) return;
	mNet->SendProtoMsg(fd, msg_id, strMsg.c_str(), strMsg.length());
}
void SeFNet::SendPbMsg(std::vector<socket_t>& fdlist, const int msg_id, ::google::protobuf::Message* pMsg)
{
	std::string strMsg = pMsg->SerializeAsString();
	if (strMsg.empty()) return;

	mNet->SendProtoMsg(fdlist, msg_id, strMsg.c_str(), strMsg.length());
}
void SeFNet::SendPBToAllMsg(const int msg_id, ::google::protobuf::Message* pMsg)
{
	std::string strMsg = pMsg->SerializeAsString();
	if (strMsg.empty()) return;
	mNet->SendProtoMsg(msg_id, strMsg.c_str(), strMsg.length());
}
