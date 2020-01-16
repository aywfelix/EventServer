
#include "ChatServerThread.h"
#include "Util.h"
#include "SePlatForm.h"

std::unique_ptr<ChatServerThread> g_pServerThread = nullptr;

bool ChatServerThread::Init()
{
	m_chat_serv.Init();
	m_chat_serv.InitHelper();
	m_chat_cli.Init();
	m_chat_cli.InitHelper();
	return true;
}

void ChatServerThread::ThreadLoop()
{
	while (IsActive())
	{
		m_chat_serv.Loop();
		m_chat_cli.Loop();

		SFSLEEP(LOOP_TIMEOUT);
	}
}
