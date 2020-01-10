#pragma  once

#include <google/protobuf/message.h>
#include "Util.h"

class Player;

class Packet
{
public:
	Packet() {}
	Packet(int msgid, ::google::protobuf::Message* msg) : msg_id(msgid), pMsg(msg) {}

	int msg_id{ -1 };
	::google::protobuf::Message* pMsg{ nullptr };
};

class PPacket
{
public:
	virtual Packet* CreatePacket(int msg_id, const char* msg, int msglen) = 0;
};

template <typename T>
class CPacket : public PPacket
{
public:
	Packet* CreatePacket(int msg_id, const char* msg, int msglen)
	{
		m_t.ParseFromArray(msg, msglen);
		m_packet.msg_id = msg_id;
		m_packet.pMsg = &m_t;
		return &m_packet;
	}
private:
	T m_t;
	Packet m_packet;
};


