#pragma once
#include "ThreadBase.h"
#include "ChatNodeClient.h"
#include "ChatNodeServer.h"

class ChatServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();

	ChatNodeClient& ChatClient() { return m_chatclient; }
	ChatNodeServer& ChatServer() { return m_chatserver; }
private:
	ChatNodeClient m_chatclient;
	ChatNodeServer m_chatserver;
};

extern std::unique_ptr<ChatServerThread> g_pServerThread;