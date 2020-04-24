#pragma once

#include "packet/PacketMgr.h"
#include "Chat.pb.h"
#include "SingleTon.hpp"

class Player;
class Packet;
extern std::unique_ptr<PacketMgr> g_pPacketMgr;
class HandleChat : public SingleTon<HandleChat>
{
public:
	enum EChat
	{
		MODULE_ID_CHAT = 11,
		RPC_CHAT_CHAT_REQ = 1101,
		RPC_CHAT_CHAT2_REQ = 1102,
	};
	HandleChat()
	{
		g_pPacketMgr->RegisterHandle(RPC_CHAT_CHAT_REQ, HandleChat::ChatReq);
		g_pPacketMgr->RegisterPacket(RPC_CHAT_CHAT_REQ, new CPacket<Chat_ChatReq>());
	}
public:
	static int ChatReq(Player* player, Packet* packet);
	static int Chat2Req(Player* player, Packet* packet);
};