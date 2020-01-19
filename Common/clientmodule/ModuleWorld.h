#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "Packet/PacketMgr.h"

class Player;
class Packet;

extern std::unique_ptr<PacketMgr> g_packetmgr;
class ModuleWorld
{
public:
	enum EWorld
	{
		MODULE_ID_WORLD = 15,
	};
	static ModuleWorld* Instance()
	{
		static ModuleWorld instance;
		return &instance;
	}
private:
};