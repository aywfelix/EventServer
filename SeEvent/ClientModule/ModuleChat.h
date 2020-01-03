#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "Packet/Packet.h"
#include "Packet/PacketMgr.h"
#include "ClientProto/Chat.pb.h"


class Player;
class Packet;

extern std::unique_ptr<PacketMgr> g_pPacketMgr;


class ModuleChat
{

public:

	enum EChat
	{
		MODULE_ID_CHAT = 13,
		RPC_CHAT_CHAT_REQ = 1301,
	};

public:
	ModuleChat()
	{
		g_pPacketMgr->RegisterHandle(RPC_CHAT_CHAT_REQ, ModuleChat::ChatReq);
		g_pPacketMgr->RegisterPacket(RPC_CHAT_CHAT_REQ, new CPacket<Chat_ChatReq>());
	}

	~ModuleChat() {}


	static ModuleChat& Instance()
	{
		static ModuleChat instance;
		return instance;
	}

public:

	static int ChatReq(Player* pPlayer, Packet* pRecvPacket);
};