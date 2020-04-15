#pragma once

#include "SingleTon.hpp"
#include "packet/PacketMgr.h"
//#include "Rank.pb.h"

class Player;
class Packet;
extern std::unique_ptr<PacketMgr> g_packetmgr;
class HandleRank : public SingleTon<HandleRank>
{
public:
	enum ELogin
	{
		MODULE_ID_LOGIN = 14,
	};
	HandleRank()
	{
	}
public:

};