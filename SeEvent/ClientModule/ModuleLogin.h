#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "Packet/Packet.h"
#include "Packet/PacketMgr.h"
//#include "ClientProto/Chat.pb.h"


class Player;
class Packet;

extern std::unique_ptr<PacketMgr> g_pPacketMgr;


class ModuleLogin
{

public:

	enum ELogin
	{
		MODULE_ID_LOGIN = 12,
	};

public:
	ModuleLogin()
	{

	}

	~ModuleLogin() {}


	static ModuleLogin& Instance()
	{
		static ModuleLogin instance;
		return instance;
	}

public:

};