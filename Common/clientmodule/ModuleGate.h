#pragma once

#include "SingleTon.hpp"
#include "packet/PacketMgr.h"

class Player;
class Packet;
extern std::unique_ptr<PacketMgr> g_packetmgr;
class ModuleGate : public SingleTon<ModuleGate>
{
public:
	enum EGate
	{
		MODULE_ID_GATE = 13,
	};
	ModuleGate()
	{

	}
private:
};