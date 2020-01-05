
#include "ChatServerThread.h"
#include "Util.h"
#include "SePlatForm.h"

std::unique_ptr<ChatServerThread> g_pServerThread = nullptr;

bool ChatServerThread::Init()
{
	m_ChatClient.Init();
	m_ChatClient.InitHelper();
	return true;
}

void ChatServerThread::ThreadLoop()
{
	while (IsActive())
	{
		m_ChatClient.Loop();
		SFSLEEP(10);
	}
}
