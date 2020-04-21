#pragma  once

#include <google/protobuf/message.h>

class Player;

class Packet
{
public:
	Packet() {}
	~Packet() 
	{
		if (msg != nullptr)
		{
			msg->Clear();
			msg = nullptr;
		}
	}
	Packet(int msg_id, ::google::protobuf::Message* msg) : msg_id(msg_id), msg(msg) {}
	int msg_id{ -1 };
	::google::protobuf::Message* msg{ nullptr };
};

class PPacket
{
public:
	virtual ~PPacket(){}
	virtual Packet* CreatePacket(int msg_id, const char* msg, int msglen) = 0;
};

template <typename T>
class CPacket : public PPacket
{
public:
	Packet* CreatePacket(int msg_id, const char* msg, int msglen)
	{
		m_t = new T;
		if (m_t == nullptr) return nullptr;
		m_t->ParseFromArray(msg, msglen);
		m_packet.msg_id = msg_id;
		m_packet.msg = m_t;
		return &m_packet;
	}
private:
	T* m_t;
	Packet m_packet;
};
