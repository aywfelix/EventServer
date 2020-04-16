#pragma once
#include "ModuleBase.h"

class ModuleBuff : public ModuleBase
{
public:
	ModuleBuff(ObjPlayer* player);
	~ModuleBuff();
	virtual void DBLoad();
	virtual void DBSave();
	virtual bool Init();
	virtual void Clear();
	virtual void Login();
	virtual void LogOut();
	virtual void HeartBeat();
	virtual EModuleType GetModuleType() { return E_MODULE_BUFF; }
private:

};

