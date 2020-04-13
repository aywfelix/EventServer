#pragma once
#include "packet/Player.h"

class ChatPlayer : public Player
{
public:
	int SendToGate(const int msg_id, ::google::protobuf::Message* pb_msg);
	int SendToGame(const int msg_id, ::google::protobuf::Message* pb_msg);

};