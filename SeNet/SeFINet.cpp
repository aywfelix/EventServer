#include "SeFINet.h"

bool ReceivePB(const int msg_id, const char* msg, const size_t msg_len, google::protobuf::Message* pb_msg)
{
	if (msg == nullptr) return false;
	return pb_msg->ParseFromArray(msg, msg_len);
}