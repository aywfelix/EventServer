#pragma once
#include "ThreadBase.h"
#include "ChatNodeClient.h"


class ChatServerThread : public ThreadBase{
public:
    bool Init();
    void ThreadLoop();

	ChatNodeClient& ChatClient()
	{
		return m_ChatClient;
	}
private:
	ChatNodeClient m_ChatClient;
};

extern std::unique_ptr<ChatServerThread> g_pServerThread;