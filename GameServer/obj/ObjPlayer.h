#pragma once
#include "Obj.h"

// сно╥╫ги╚
class ObjPlayer : public Obj
{
public:
	ObjPlayer();
	~ObjPlayer();
	virtual void HeartBeat(time_t ti);
	virtual void EnterScene() {}
	virtual void LeaveScene() {}
	virtual void OnEnterScene() {}
	virtual void OnLeaveScene() {}
	virtual EObjType GetObjType() { return E_OBJ_PLAYER; }

private:

};

