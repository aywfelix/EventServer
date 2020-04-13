#pragma once

class Context;
enum EState
{
	E_STATE_INVALID = 0,
	E_STATE_STAND = 1,
	E_STATE_WALK = 2,
	E_STATE_RUN = 3,
	E_STATE_MAX = 4,
};

class IState
{
public:
	virtual void Action(Context* context) {}
	virtual EState GetType() { return E_STATE_INVALID; }
};

class StandState : public IState
{
public:
	virtual void Action(Context* context);
	virtual EState GetType() { return E_STATE_STAND; }
private:
};

class WalkState : public IState
{
public:
	virtual void Action(Context* context);
	virtual EState GetType() { return E_STATE_WALK; }
private:
};

class RunState : public IState
{
public:
	virtual void Action(Context* context);
	virtual EState GetType() { return E_STATE_RUN; }
private:
};


