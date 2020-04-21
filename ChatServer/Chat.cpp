#include "Chat.h"
#include "NodeNet/ChatServerThread.h"
#include "packet/PacketMgr.h"
#include "client/chat/handleChat.h"
#include "Session.h"

Chat::Chat()
{

}
Chat::~Chat()
{

}

void Chat::Init()
{
	g_pPacketMgr = std::make_unique<PacketMgr>();
	g_pServerThread = std::make_unique<ChatServerThread>();
	g_pSessionPool = std::make_unique<SessionPool>();
	InitManager();
}

void Chat::Start()
{
	g_pServerThread->Start();
}

void Chat::Stop()
{
	g_pServerThread->Stop();

}

void Chat::InitManager()
{
	// register chat msg
	HandleChat::Instance();
}