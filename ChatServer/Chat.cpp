#include "Chat.h"
#include "NodeNet/ChatServerThread.h"
#include "packet/PacketMgr.h"
#include "clientmodule/ModuleChat.h"
#include "Session.h"

Chat::Chat()
{

}
Chat::~Chat()
{

}

void Chat::Init()
{
	g_pServerThread.reset(new ChatServerThread());
	g_packetmgr.reset(new PacketMgr);
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
	ModuleChat::Instance();
}