#pragma once
#include <google/protobuf/message.h>
#include "MemPool.hpp"

class Packet;

class Player : public MemElem
{
public:
	Player();
	virtual ~Player();
	virtual bool Init();

	virtual bool HeartBeat();
	virtual int SendMsg(unsigned short msg_id, ::google::protobuf::Message* pMsg);
	virtual int SendMsg(Packet* pPacket);

protected:
	//virtual int onReciveMsg(Msg_Head* pHead, char* strContent) = 0;
};