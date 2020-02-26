#include "ObjBase.h"
ObjBase::ObjBase()
{

}

ObjBase::~ObjBase()
{

}

void ObjBase::HeartBeat(time_t ti)
{
	UpdateTime(ti);
}

void ObjBase::UpdateTime(time_t ti)
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
