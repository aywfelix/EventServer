#include "Chat.h"
#include "NodeNet/ChatServerThread.h"
#include "Packet/PacketMgr.h"
#include "ClientModule/ModuleChat.h"

Chat::Chat()
{

}
Chat::~Chat()
{

}

void Chat::Init()
{
	g_pServerThread.reset(new ChatServerThread());
	g_pPacketMgr.reset(new PacketMgr);

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
