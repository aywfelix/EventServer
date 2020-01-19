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
		MODULE_ID_CHAT = 11,
		RPC_CHAT_CHAT_REQ = 1101,
	};
	ModuleChat()
	{
		g_packetmgr->RegisterHandle(RPC_CHAT_CHAT_REQ, ModuleChat::ChatReq);
		g_packetmgr->RegisterPacket(RPC_CHAT_CHAT_REQ, new CPacket<Chat_ChatReq>());
	}
	static ModuleChat* Instance()
	{
		static ModuleChat instance;
		return &instance;
	}
public:
	static int ChatReq(Player* player, Packet* recv_packet);
};