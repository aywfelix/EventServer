#pragma once

#include "SingleTon.hpp"
#include "Packet/PacketMgr.h"
#include "ClientProto/Chat.pb.h"

class Player;
class Packet;

class ModuleChat : public SingleTon<ModuleChat>
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

public:
	static int ChatReq(Player* player, Packet* recv_packet);
};