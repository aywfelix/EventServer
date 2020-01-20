#pragma once

#include "SingleTon.hpp"
#include "packet/PacketMgr.h"

class Player;
class Packet;

enum EGame
{
	MODULE_ID_GAME = 12,
};

extern std::unique_ptr<PacketMgr> g_packetmgr;
class ModuleGame : public SingleTon<ModuleGame>
{
public:
	ModuleGame()
	{

	}
public:
};