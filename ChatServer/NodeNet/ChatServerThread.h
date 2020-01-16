#pragma once
#include "ThreadBase.h"
#include "ChatNodeClient.h"
#include "ChatNodeServer.h"

class ChatServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();

	ChatNodeClient& ChatClient() { return m_chat_cli; }
	ChatNodeServer& ChatServer() { return m_chat_serv; }
private:
	ChatNodeClient m_chat_cli;
	ChatNodeServer m_chat_serv;
};

extern std::unique_ptr<ChatServerThread> g_pServerThread;