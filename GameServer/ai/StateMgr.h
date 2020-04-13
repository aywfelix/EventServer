#pragma once
#include "State.h"
#include <memory>

class StateMgr
{
public:
	StateMgr()
	{
		m_states[E_STATE_INVALID] = nullptr;
		m_states[E_STATE_STAND] = new StandState;
		m_states[E_STATE_WALK] = new WalkState;
		m_states[E_STATE_RUN] = new RunState;
	}
	~StateMgr()
	{
		for (int i = E_STATE_INVALID; i < E_STATE_MAX; i++)
		{
			if (m_states[i])
			{
				delete m_states[i];
				m_states[i] = nullptr;
			}
		}
	}

	IState* GetState(EState statetype)
	{
		if (m_states[statetype])
			return m_states[statetype];
		else
			return nullptr;
	}
private:
	IState* m_states[E_STATE_MAX];
};

extern std::unique_ptr<StateMgr> g_pStateMgr;

