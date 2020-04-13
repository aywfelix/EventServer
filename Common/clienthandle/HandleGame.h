#pragma once

#include "SingleTon.hpp"
#include "packet/PacketMgr.h"

class Player;
class Packet;
extern std::unique_ptr<PacketMgr> g_packetmgr;
class HandleGame : public SingleTon<HandleGame>
{
public:
	enum EGame
	{
		MODULE_ID_GAME = 12,
	};
	HandleGame()
	{

	}
public:
};