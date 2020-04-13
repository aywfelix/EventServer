#pragma once

#include "SingleTon.hpp"
#include "packet/PacketMgr.h"
#include "clientproto/Chat.pb.h"

class Player;
class Packet;
extern std::unique_ptr<PacketMgr> g_packetmgr;
class HandleChat : public SingleTon<HandleChat>
{
public:
	enum EChat
	{
		MODULE_ID_CHAT = 11,
		RPC_CHAT_CHAT_REQ = 1101,
	};
	HandleChat()
	{
		g_packetmgr->RegisterHandle(RPC_CHAT_CHAT_REQ, HandleChat::ChatReq);
		g_packetmgr->RegisterPacket(RPC_CHAT_CHAT_REQ, new CPacket<Chat_ChatReq>());
	}
public:
	static int ChatReq(Player* player, Packet* recv_packet);
};