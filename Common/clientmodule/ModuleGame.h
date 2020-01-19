#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "Packet/PacketMgr.h"

class Player;
class Packet;

extern std::unique_ptr<PacketMgr> g_packetmgr;
class ModuleGame
{
public:
	enum EGame
	{
		MODULE_ID_GAME = 12,
	};
public:
	static ModuleGame& Instance()
	{
		static ModuleGame instance;
		return instance;
	}
public:
};