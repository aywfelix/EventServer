#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "Packet/PacketMgr.h"

class Player;
class Packet;

extern std::unique_ptr<PacketMgr> g_packetmgr;

class ModuleGate
{
public:
	enum EGate
	{
		MODULE_ID_GATE = 13,
	};
public:
	static ModuleGate& Instance()
	{
		static ModuleGate instance;
		return instance;
	}
};