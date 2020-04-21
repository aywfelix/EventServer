#include "Client.h"
#include "JsonConfig.h"
#include "Util.h"
#include "SeFNetClient.h"
#include "LogUtil.h"

// 通信协议
#include "client/chat/Chat.pb.h"
#include "client/chat/HandleChat.h"
#include "client/login/Login.pb.h"
#include "client/login/HandleLogin.h"


enum ChatChannelType
{
	CHAT_CHANNEL_GM = 1,
	CHAT_CHANNEL_SYSTEM = 2,
};

bool Client::Init()
{
	m_pNetClientModule = new SeFNetClient();
	m_pNetClientModule->AddEventCallBack(ServerType::SERVER_TYPE_GATE, this, &Client::OnSocketEvent);
	m_pNetClientModule->AddReceiveCallBack(ServerType::SERVER_TYPE_GATE, this, &Client::OnMessage);
	
	ConnectServer();
	m_ServerId = g_pConfig->m_Root["GatePlayerServer"]["NodeId"].asInt();
	return true;
}

void Client::ConnectServer()
{
	//set gate server info
	ConnectDataPtr ServerData = std::make_shared<ConnectData>();
	ServerData->serv_id = g_pConfig->m_Root["GatePlayerServer"]["NodeId"].asInt();
	ServerData->port = g_pConfig->m_Root["GatePlayerServer"]["NodePort"].asInt();
	ServerData->serv_name = g_pConfig->m_Root["GatePlayerServer"]["NodeName"].asString();
	ServerData->ip = g_pConfig->m_Root["GatePlayerServer"]["NodeIp"].asString();
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
		//Chat_ChatReq chat_msg;
		//chat_msg.set_channel(ChatChannelType::CHAT_CHANNEL_GM);
		//chat_msg.set_chat_msg("this is gm msg to game player");
		//chat_msg.SerializeToString(&send_msg);
		//m_pNetClientModule->SendByServId(m_ServerId, (const uint16_t)(HandleChat::RPC_CHAT_CHAT_REQ), send_msg.c_str(), send_msg.length());
	
		////send msg to gate
		//chat_msg.set_channel(ChatChannelType::CHAT_CHANNEL_SYSTEM);
		//chat_msg.set_chat_msg("this is system msg to system");
		//chat_msg.SerializeToString(&send_msg);
		//m_pNetClientModule->SendByServId(m_ServerId, (const uint16_t)(HandleChat::RPC_CHAT_CHAT_REQ), send_msg.c_str(), send_msg.length());
		
		// 验证登录信息
		Login_LoginReq login_msg;
		login_msg.set_login_name("aa");
		login_msg.set_login_pwd("123456");
		login_msg.SerializeToString(&send_msg);
		m_pNetClientModule->SendByServId(m_ServerId, (const uint16_t)(HandleLogin::RPC_LOGIN_LOGIN_REQ), send_msg.c_str(), send_msg.length());
	}
}

void Client::OnMessage(const socket_t sock_fd, const int msg_id, const char* msg, const size_t msg_len)
{
	//Chat_ChatReply reply;
	Login_LoginReply reply;
	reply.ParseFromArray(msg, msg_len);
	CLOG_INFO << "client recv msg from gate :" << (int)(reply.ret()) << CLOG_END;
}

