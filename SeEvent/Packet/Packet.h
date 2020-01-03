#pragma  once

#include <google/protobuf/message.h>
#include "Util.h"

class Player;

class Packet
{
public:
	Packet() {}
	Packet(int msgid, ::google::protobuf::Message* msg) : msg_id(msgid), pMsg(msg) {}
	~Packet()
	{
		DELETE_PTR(pMsg);
	}
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
		T* pMsg = new T;
		Packet* packet = new Packet;
		if (pMsg == nullptr || packet == nullptr)
		{
			return nullptr;
		}
		pMsg->ParseFromArray(msg, msglen);
		packet->msg_id = msg_id;
		packet->pMsg = pMsg;
		return packet;
	}

private:
};


