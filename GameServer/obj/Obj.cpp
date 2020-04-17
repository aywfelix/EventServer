#include "Obj.h"
#include "scene/map/Scene.h"

Obj::Obj()
{
	m_context.SetContext(this, E_STATE_INVALID);
}

Obj::~Obj()
{

}

void Obj::HeartBeat(time_t ti)
{
	UpdateTime(ti);
	m_context.DoAction();   // 只要调用context类ChangeState函数就执行相应的动作
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

bool Obj::UpdateZone()
{
	m_scene = GetScene();
	return true;
}