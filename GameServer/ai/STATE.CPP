#include "State.h"
#include "Context.h"

void StandState::Action(Context* context)
{
	//context->GetObj()->Action();
	context->ChangeState(E_STATE_WALK);
}

void WalkState::Action(Context* context)
{
	//context->GetObj()->Action();
	context->ChangeState(E_STATE_RUN);
}

void RunState::Action(Context* context)
{
	//context->GetObj()->Action();
	context->ChangeState(E_STATE_STAND);
}

