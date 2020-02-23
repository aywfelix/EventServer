#pragma once
#include <google/protobuf/message.h>
#include "MemPool.hpp"

class Packet;

class Player : public MemElem
{
public:
	Player();
	virtual ~Player();
	virtual bool Init();

	virtual bool HeartBeat();
	virtual int SendMsg(unsigned short msg_id, ::google::protobuf::Message* pb_msg);
	virtual int SendMsg(Packet* pPacket);

	uint64_t m_playerid;
	int32_t m_servid;
};