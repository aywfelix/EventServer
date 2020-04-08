#include "Obj.h"
Obj::Obj()
{

}

Obj::~Obj()
{

}

void Obj::HeartBeat(time_t ti)
{
	UpdateTime(ti);
}

void Obj::UpdateTime(time_t ti)
{
	if (m_last == 0)
	{
		m_last = ti;
	}
	else
	{
		m_last = m_now;
	}
	m_now = ti;
}

void Obj::SendMsg(uint32_t msgid, ::google::protobuf::Message* msg)
{
	if (!IsPlayer()) return;

}