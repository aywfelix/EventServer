#pragma once
#include "Obj.h"
#include "common/GameDef.h"
#include "module/ModuleMgr.h"

class ModuleBase;
// сно╥╫ги╚
class ObjPlayer : public Obj
{
public:
	ObjPlayer();
	~ObjPlayer();
	virtual bool Init();
	void DBLoad();
	void DBSave();
	void Clear();
	void Login();
	void LogOut();
	virtual void HeartBeat(time_t ti);
	virtual void EnterScene() {}
	virtual void LeaveScene() {}
	virtual void OnEnterScene() {}
	virtual void OnLeaveScene() {}
	virtual EObjType GetObjType() { return E_OBJ_PLAYER; }

	ModuleBase* GetModule(EModuleType type) { return m_module_mgr.GetModule(type); }
private:
	ModuleMgr m_module_mgr;
};

