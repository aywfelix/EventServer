
#include "Player.h"

Player::Player()
{}
Player::~Player()
{}
bool Player::Init()
{
	return true;
}

bool Player::HeartBeat()
{
	return true;
}
int Player::SendMsg(unsigned short msg_id, ::google::protobuf::Message* pb_msg)
{
	return 0;
}
int Player::SendMsg(Packet* pPacket)
{
	return 0;
}