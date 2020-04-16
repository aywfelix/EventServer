#pragma once
#include <vector>
#include "ModuleBase.h"

class ModuleMgr
{
public:
	ModuleMgr();
	~ModuleMgr();
	// 模块必须实现接口
	void DBLoad();
	void DBSave();
	bool Init();
	void Clear();
	void Login();
	void LogOut();
	void HeartBeat();
	void AddModule(ModuleBase* module);
	void DelModule(EModuleType type);
	void DelModule(ModuleBase* module);
	ModuleBase* GetModule(EModuleType type);

private:
	std::vector<ModuleBase*> m_modules;
};

