#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "Packet/Packet.h"
#include "Packet/PacketMgr.h"
#include "ClientProto/Chat.pb.h"


class Player;
class Packet;

extern std::unique_ptr<PacketMgr> g_packetmgr;


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
		g_packetmgr->RegisterHandle(RPC_CHAT_CHAT_REQ, ModuleChat::ChatReq);
		g_packetmgr->RegisterPacket(RPC_CHAT_CHAT_REQ, new CPacket<Chat_ChatReq>());
	}

	~ModuleChat() {}


	static ModuleChat& Instance()
	{
		static ModuleChat instance;
		return instance;
	}

public:

	static int ChatReq(Player* player, Packet* recv_packet);
};