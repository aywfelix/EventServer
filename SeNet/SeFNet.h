#pragma once

#include <list>
#include <map>
#include <google/protobuf/message.h>
#include "SeFINet.h"

class SeNet;

class SeFNet
{
public:
	void Execute(LOOP_RUN_TYPE run);

	// init client
	bool InitNet(const char* ip, uint32_t port);
	// init server 
	bool InitNet(uint32_t port);

	// add callback
	template<typename BaseType>
	void AddReceiveCallBack(const int msg_id, BaseType* pBase, void (BaseType::* HandleReceive)(const socket_t, const int, const char*, const size_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(HandleReceive, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr = std::make_shared<NET_RECEIVE_FUNCTOR>(functor);
		AddReceiveCallBack(msg_id, functorPtr);
	}

	template<typename BaseType>
	void AddReceiveCallBack(BaseType* pBase, void (BaseType::* HandleReceive)(const socket_t, const int, const char*, const size_t))
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
	void AddReceiveCallBack(const int msg_id, const NET_RECEIVE_FUNCTOR_PTR& cb);
	void AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb);
	void AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb);
	void RemoveReceiveCallBack(const int msg_id);

	// send msg by fd
	void SendMsg(const socket_t fd, const int msg_id, const char* msg, int len);
	void SendMsg(std::vector<socket_t>& fdlist, const int msg_id, const char* msg, int len);
	void SendToAll(const int msg_id, const char* msg, int len);

	void SendPbMsg(const socket_t fd, const int msg_id, ::google::protobuf::Message* pb_msg);
	void SendPbMsg(std::vector<socket_t>& fdlist, const int msg_id, ::google::protobuf::Message* pb_msg);
	void SendPBToAllMsg(const int msg_id, ::google::protobuf::Message* pb_msg);

	SeNet* GetNet() { return mNet; }
private:
	void OnReceiveNetPack(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len);
	void OnSocketNetEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet);
private:
	SeNet* mNet;
	// msg_id  msghandle
	std::map<int, NET_RECEIVE_FUNCTOR_PTR> mReceiveCallBack;
	std::list<NET_RECEIVE_FUNCTOR_PTR> mReceiveCallBackList;
	std::list<NET_EVENT_FUNCTOR_PTR> mEventCallBackList;
};

// 客户端需要连接服务器信息
struct ConnectData
{
	int serv_id;
	int port;
	std::string ip;
	std::string serv_name;
	ServerType serv_type;
	ConnectState conn_state;
	socket_t sock_fd;
	std::shared_ptr<SeFNet> pNet;
};

using ConnectDataPtr = std::shared_ptr<ConnectData>;