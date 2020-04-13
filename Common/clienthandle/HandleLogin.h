#pragma once

#include "packet/PacketMgr.h"
#include "clientproto/Login.pb.h"
#include "SingleTon.hpp"

class Player;
class Packet;
extern std::unique_ptr<PacketMgr> g_packetmgr;
class HandleGate : public SingleTon<HandleGate>
{
public:
	enum ELogin
	{
		MODULE_ID_LOGIN = 14,
		RPC_LOGIN_LOGIN_REQ = 1401,
		RPC_LOGIN_CREATEROLE_REQ = 1402,
		RPC_LOGIN_SELECTROLE_REQ = 1403,
		RPC_LOGIN_DELROLE_REQ = 1404,
	};
	HandleGate()
	{
		g_packetmgr->RegisterHandle(RPC_LOGIN_LOGIN_REQ, HandleGate::LoginReq);
		g_packetmgr->RegisterPacket(RPC_LOGIN_LOGIN_REQ, new CPacket<Login_LoginReq>());
		g_packetmgr->RegisterHandle(RPC_LOGIN_CREATEROLE_REQ, HandleGate::CreateRoleReq);
		g_packetmgr->RegisterPacket(RPC_LOGIN_CREATEROLE_REQ, new CPacket<Login_CreateRoleReq>());
		g_packetmgr->RegisterHandle(RPC_LOGIN_SELECTROLE_REQ, HandleGate::SelectRoleReq);
		g_packetmgr->RegisterPacket(RPC_LOGIN_SELECTROLE_REQ, new CPacket<Login_SelectRoleReq>());
		g_packetmgr->RegisterHandle(RPC_LOGIN_DELROLE_REQ, HandleGate::DelRoleReq);
		g_packetmgr->RegisterPacket(RPC_LOGIN_DELROLE_REQ, new CPacket<Login_DelRoleReq>());
	}
public:
	static int LoginReq(Player* player, Packet* packet);
	static int CreateRoleReq(Player* player, Packet* packet);
	static int SelectRoleReq(Player* player, Packet* packet);
	static int DelRoleReq(Player* player, Packet* packet);
};