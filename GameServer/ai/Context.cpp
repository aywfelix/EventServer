#include "Context.h"
#include "obj/Obj.h"
#include "State.h"
#include "StateMgr.h"

void Context::ChangeState(EState state)
{
	if (m_state != state)
	{
		m_state = state;
	}
}

void Context::DoAction()
{
	if (m_last_state && m_last_state == m_state)
		return;
	g_pStateMgr->GetState(m_state)->Action(this);
	m_last_state = m_state;
}