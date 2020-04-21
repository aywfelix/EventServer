#pragma once

#include "SingleTon.hpp"
#include "packet/PacketMgr.h"

class Player;
class Packet;
extern std::unique_ptr<PacketMgr> g_pPacketMgr;
class HandleWorld : public SingleTon<HandleWorld>
{
public:
	enum EWorld
	{
		MODULE_ID_WORLD = 13,
	};
	HandleWorld()
	{

	}
private:
};