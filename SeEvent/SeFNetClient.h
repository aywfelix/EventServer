#pragma once

#include <list>
#include <map>

#include "SeFINet.h"
#include "MapEx.hpp"
#include "SeFNet.h"

class SeNet;
// 这是一个socket客户端管理类
class SeFNetClient
{
public:
	SeFNetClient();
	~SeFNetClient();

	void AddServer(ConnectDataPtr& info);
	void Execute(LOOP_RUN_TYPE run);

	// add call back
	template<typename BaseType>
	void AddReceiveCallBack(ServerType eType, const uint32_t nMsgId, BaseType* pBase, void(BaseType::* HandleReceive)(const socket_t, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(HandleReceive, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr = std::make_shared<NET_RECEIVE_FUNCTOR>(functor);
		AddReceiveCallBack(eType, nMsgId, functorPtr);
	}
	template<typename BaseType>
	void AddReceiveCallBack(ServerType eType, BaseType* pBase, void(BaseType::* HandleReceive)(const socket_t, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(HandleReceive, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr = std::make_shared<NET_RECEIVE_FUNCTOR>(functor);
		AddReceiveCallBack(eType, functorPtr);
	}
	template<typename BaseType>
	void AddEventCallBack(ServerType eType, BaseType* pBase, void(BaseType::* HandleEvent)(const socket_t nFd, const SE_NET_EVENT nEvent, SeNet* pNet))
	{
		NET_EVENT_FUNCTOR functor = std::bind(HandleEvent, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NET_EVENT_FUNCTOR_PTR functorPtr = std::make_shared<NET_EVENT_FUNCTOR>(functor);
		AddEventCallBack(eType, functorPtr);
	}
	// remove call back
	void RemoveReceiveCallBack(ServerType eType, uint32_t nMsgId);
	// send msg
	void SendByServId(int nServerId, const int nMsgID, const char* msg, int len);
	void SendByServIds(std::vector<int>& nServerIds, const int nMsgID, const char* msg, int len);
	void SendPbByServId(int nServerId, const int nMsgID, ::google::protobuf::Message* pMsg);
	void SendPbByServIds(std::vector<int>& nServerIds, const int nMsgID, ::google::protobuf::Message* pMsg);
	void SendToAll(const int nMsgID, const char* msg, int len);
	void SendPBToAll(const int nMsgID, ::google::protobuf::Message* pMsg);
	
	void SendByServType(ServerType type, const int nMsgID, const char* msg, int len);
	void SendByServTypes(std::vector<ServerType>& types, const int nMsgID, const char* msg, int len);
	void SendPbByServType(ServerType type, const int nMsgID, ::google::protobuf::Message* pMsg);
	void SendPbByServTypes(std::vector<ServerType>& types, const int nMsgID, ::google::protobuf::Message* pMsg);
	// get server info
	ConnectDataPtr GetServerNetInfo(const int& nServerID);
	ConnectDataPtr GetServerNetInfo(const SeNet* pNet);
	ConnectDataPtr GetServerNetInfo(const socket_t& nFd);
private:
	void AddReceiveCallBack(ServerType eType, uint32_t nMsgId, NET_RECEIVE_FUNCTOR_PTR functorPtr);
	void AddReceiveCallBack(ServerType eType, NET_RECEIVE_FUNCTOR_PTR functorPtr);
	void AddEventCallBack(ServerType eType, NET_EVENT_FUNCTOR_PTR functorPtr);
	void ProcessExecute(LOOP_RUN_TYPE run);
	void ProcessAddConnect();
	void InitCallBacks(ConnectDataPtr& data);
private:
	struct CallBack
	{// msgid handle function
		std::map<int, NET_RECEIVE_FUNCTOR_PTR> mReceiveCallBack;
		std::list<NET_RECEIVE_FUNCTOR_PTR> mCallBackList;
		std::list<NET_EVENT_FUNCTOR_PTR> mEventCallBackList;
	};
	// ServerType callback
	std::map<int, CallBack> mmCallBack;

	std::list<ConnectDataPtr> mTemplist;
	// serverid connect data
	std::map<int, ConnectDataPtr> mConnecServers; // 
	// server type    serverid  connect data
	std::map<int, ConsistentHashEx<int, ConnectDataPtr>> mConsistentServers;
};

