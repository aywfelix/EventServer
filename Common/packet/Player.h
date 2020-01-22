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

	std::string m_playerid;
	std::string m_servid;
};