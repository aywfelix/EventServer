#pragma once
#include "packet/Player.h"

class ChatPlayer : public Player
{
public:
	int SendToGate(unsigned short msg_id, ::google::protobuf::Message* pb_msg);
	int SendToGame(unsigned short msg_id, ::google::protobuf::Message* pb_msg);

};