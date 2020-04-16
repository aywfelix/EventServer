#include "ObjPlayer.h"
#include "module/ModuleBuff.h"
#include "module/ModulePet.h"
#include "module/ModuleRank.h"

ObjPlayer::ObjPlayer()
{
}
ObjPlayer::~ObjPlayer()
{
	m_module_mgr.Clear();
}

bool ObjPlayer::Init()
{
	ModuleBuff* module_buff = new ModuleBuff(this);
	//ModulePet* module_pet = new ModulePet(this);
	//ModuleRank* module_rank = new ModuleRank(this);
	m_module_mgr.AddModule(module_buff);
	//m_module_mgr.AddModule(module_pet);
	//m_module_mgr.AddModule(module_rank);
	m_module_mgr.Init();
	return true;
}

void ObjPlayer::HeartBeat(time_t ti)
{
	Obj::HeartBeat(ti);
	m_module_mgr.HeartBeat();
}

void ObjPlayer::DBLoad()
{
	m_module_mgr.DBLoad();
}
void ObjPlayer::DBSave()
{
	m_module_mgr.DBSave();
}
void ObjPlayer::Clear()
{
	m_module_mgr.Clear();
}
void ObjPlayer::Login()
{
	m_module_mgr.Login();
}
void ObjPlayer::LogOut()
{
	m_module_mgr.LogOut();
}