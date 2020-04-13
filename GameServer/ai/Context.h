#pragma once
#include "State.h"
class Obj;
class Context
{
public:
	void SetContext(Obj* obj, EState state)
	{
		m_obj = obj;
		m_state = state;
		m_last_state = E_STATE_INVALID;
	}

	void ChangeState(EState state);
	void DoAction();
	Obj* GetObj() { return m_obj; }
private:
	Obj* m_obj;
	EState m_state;
	EState m_last_state;
};
