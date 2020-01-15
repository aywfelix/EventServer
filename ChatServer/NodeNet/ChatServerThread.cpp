
#include "ChatServerThread.h"
#include "Util.h"
#include "SePlatForm.h"

std::unique_ptr<ChatServerThread> g_pServerThread = nullptr;

bool ChatServerThread::Init()
{
	m_chatserver.Init();
	m_chatserver.InitHelper();
	m_chatclient.Init();
	m_chatclient.InitHelper();
	return true;
}

void ChatServerThread::ThreadLoop()
{
	while (IsActive())
	{
		m_chatserver.Loop();
		m_chatclient.Loop();

		SFSLEEP(LOOP_TIMEOUT);
	}
}
