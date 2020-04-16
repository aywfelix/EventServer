#pragma once
#include "ModuleBase.h"

class ModuleRank : public ModuleBase
{
public:
	ModuleRank(ObjPlayer* player);
	~ModuleRank();
	virtual void DBLoad();
	virtual void DBSave();
	virtual bool Init();
	virtual void Clear();
	virtual void Login();
	virtual void LogOut();
	virtual void HeartBeat();
	virtual EModuleType GetModuleType() { return E_MODULE_RANK; }
private:
};

