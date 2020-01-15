#pragma once
#include "Packet/Player.h"

class ChatPlayer : public Player
{
public:
	int SendToGate(unsigned short msg_id, ::google::protobuf::Message* pMsg);
	int SendToGame(unsigned short msg_id, ::google::protobuf::Message* pMsg);

	uint64_t m_PlayerId;
	int m_ServerId;
};