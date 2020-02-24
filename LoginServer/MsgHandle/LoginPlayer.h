#pragma once

#include "packet/Player.h"

class LoginPlayer : public Player
{
public:
	int SendToGate(unsigned short msg_id, ::google::protobuf::Message* pb_msg);
	int SendToWorld(unsigned short msg_id, ::google::protobuf::Message* pb_msg);

};

