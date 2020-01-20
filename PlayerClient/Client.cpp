#include "Client.h"
#include "JsonConfig.h"
#include "clientproto/Chat.pb.h"
#include "clientmodule/ModuleChat.h"
#include "Util.h"
#include "SeFNetClient.h"
#include "LogHelper.h"

bool Client::Init()
{
	m_pNetClientModule = new SeFNetClient();
	m_pNetClientModule->AddEventCallBack(ServerType::SERVER_TYPE_GATE, this, &Client::OnSocketEvent);
	m_pNetClientModule->AddReceiveCallBack(ServerType::SERVER_TYPE_GATE, this, &Client::OnMessage);
	
	ConnectServer();
	m_ServerId = g_JsonConfig->m_Root["GatePlayerServer"]["NodeId"].asInt();
	return true;
}

void Client::ConnectServer()
{
	//set gate server info
	ConnectDataPtr ServerData = std::make_shared<ConnectData>();
	ServerData->serv_id = g_JsonConfig->m_Root["GatePlayerServer"]["NodeId"].asInt();
	ServerData->port = g_JsonConfig->m_Root["GatePlayerServer"]["NodePort"].asInt();
	ServerData->serv_name = g_JsonConfig->m_Root["GatePlayerServer"]["NodeName"].asString();
	ServerData->ip = g_JsonConfig->m_Root["GatePlayerServer"]["NodeIp"].asString();
	ServerData->serv_type = ServerType::SERVER_TYPE_GATE;
	ServerData->conn_state = ConnectState::CONNECTING;
	m_pNetClientModule->AddServer(ServerData);
}

bool Client::Loop()
{
	while (1)
	{
		m_pNetClientModule->Execute(LOOP_RUN_NONBLOCK);
		sf_sleep(LOOP_TIMEOUT);
	}
}

void Client::OnSocketEvent(const socket_t sock_fd, const SE_NET_EVENT nEvent, SeNet* pNet)
{
	if (nEvent == SE_NET_EVENT_CONNECTED)
	{
		CLOG_INFO << "client connect to gate ok" << CLOG_END;
		//send msg to gate
		std::string send_msg;
		Chat_ChatReq chat_msg;
		chat_msg.set_channel(ChatChannelType::CHAT_CHANNEL_GM);
		chat_msg.set_chat_msg("this is gm msg to game player");
		chat_msg.SerializeToString(&send_msg);
		m_pNetClientModule->SendByServId(m_ServerId, (const uint16_t)(ModuleChat::RPC_CHAT_CHAT_REQ), send_msg.c_str(), send_msg.length());
	
		//send msg to gate
		chat_msg.set_channel(ChatChannelType::CHAT_CHANNEL_SYSTEM);
		chat_msg.set_chat_msg("this is system msg to system");
		chat_msg.SerializeToString(&send_msg);
		m_pNetClientModule->SendByServId(m_ServerId, (const uint16_t)(ModuleChat::RPC_CHAT_CHAT_REQ), send_msg.c_str(), send_msg.length());
	}
}

void Client::OnMessage(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	Chat_ChatReply reply;
	reply.ParseFromArray(msg, msg_len);
	CLOG_INFO << "client recv msg from gate :" << reply.chat_msg() <<CLOG_END;
}

