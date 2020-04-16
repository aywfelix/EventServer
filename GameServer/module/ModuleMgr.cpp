#include "ModuleMgr.h"

ModuleMgr::ModuleMgr()
{
}
ModuleMgr::~ModuleMgr()
{

	m_modules.clear();
}

bool ModuleMgr::Init()
{
	for (auto& it : m_modules)
	{
		if (!it->Init())
			return false;
	}
	return true;
}

void ModuleMgr::Clear()
{
	for (auto& it : m_modules)
	{
		it->Clear();
	}
}
void ModuleMgr::DBLoad()
{
	for (auto& it : m_modules)
	{
		it->DBLoad();
	}
}
void ModuleMgr::DBSave()
{
	for (auto& it : m_modules)
	{
		it->DBSave();
	}
}
void ModuleMgr::HeartBeat()
{
	for (auto& it : m_modules)
	{
		it->HeartBeat();
	}
}
void ModuleMgr::Login()
{
	for (auto& it : m_modules)
	{
		it->Login();
	}
}
void ModuleMgr::LogOut()
{
	for (auto& it : m_modules)
	{
		it->LogOut();
	}
}
void ModuleMgr::AddModule(ModuleBase* module)
{
	if(module) m_modules.push_back(module);
}
void ModuleMgr::DelModule(EModuleType type)
{
	for (auto it = m_modules.begin(); it != m_modules.end();it++)
	{
		if ((*it)->GetModuleType() == type)
		{
			(*it)->Clear();
			m_modules.erase(it);
			return;
		}
	}
}
void ModuleMgr::DelModule(ModuleBase* module)
{
	for (auto it = m_modules.begin(); it != m_modules.end();it++)
	{
		if ((*it) == module)
		{
			(*it)->Clear();
			m_modules.erase(it);
			return;
		}
	}
}
ModuleBase* ModuleMgr::GetModule(EModuleType type)
{
	for (auto& it : m_modules)
	{
		if (it->GetModuleType() == type)
			return it;
	}
	return nullptr;
}

