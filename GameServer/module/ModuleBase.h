#pragma once

#include "common/GameDef.h"

class ObjPlayer;
// 各个玩法模块统一接口类
class ModuleBase
{
public:
	ModuleBase(ObjPlayer* player);
	virtual ~ModuleBase();
	virtual void DBLoad() = 0;
	virtual void DBSave() = 0;
	virtual bool Init() = 0;
	virtual void Clear() = 0;
	virtual void Login() = 0;
	virtual void LogOut() = 0;
	virtual void HeartBeat() = 0;
	virtual EModuleType GetModuleType() { return E_MODULE_VOID; }
	ObjPlayer* GetPlayer() { return m_obj_player; }
protected:
	ObjPlayer* m_obj_player;
};

