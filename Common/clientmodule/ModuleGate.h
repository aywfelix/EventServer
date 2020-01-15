#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "Packet/Packet.h"
#include "Packet/PacketMgr.h"
//#include "ClientProto/Chat.pb.h"


class Player;
class Packet;

extern std::unique_ptr<PacketMgr> g_packetmgr;


class ModuleGate
{

public:

	enum EGate
	{
		MODULE_ID_GATE = 11,
	};

public:
	ModuleGate()
	{

	}

	~ModuleGate() {}


	static ModuleGate& Instance()
	{
		static ModuleGate instance;
		return instance;
	}

public:

};