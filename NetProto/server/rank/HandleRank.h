#pragma once

#include "SingleTon.hpp"
#include "packet/PacketMgr.h"
//#include "Rank.pb.h"

class Player;
class Packet;
extern std::unique_ptr<PacketMgr> g_pPacketMgr;
class HandleRank : public SingleTon<HandleRank>
{
public:
	enum ERank
	{
		MODULE_ID_RANK = 14,
	};
	HandleRank()
	{
	}
public:

};