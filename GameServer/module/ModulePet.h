#pragma once
#include "ModuleBase.h"

class ModulePet : public ModuleBase
{
public:
	ModulePet(ObjPlayer* player);
	~ModulePet();
	virtual void DBLoad();
	virtual void DBSave();
	virtual bool Init();
	virtual void Clear();
	virtual void Login();
	virtual void LogOut();
	virtual void HeartBeat();
	virtual EModuleType GetModuleType() { return E_MODULE_PET; }
private:
};
