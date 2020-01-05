#pragma once
#include "Packet/Player.h"

class ChatPlayer : public Player
{
public:
	int SendMsg(unsigned short msg_id, ::google::protobuf::Message* pMsg);

	uint64_t m_PlayerId;
	int m_ServerId;
};