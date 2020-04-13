#pragma once
#include "common/GameDef.h"
#include "MemPool.hpp"
#include "ai/Context.h"

class Scene;
// 所有游戏对象基类
class Obj : public MemElem
{
public:
	Obj();
	virtual ~Obj();
	
	virtual void HeartBeat(time_t ti);
	virtual void EnterScene() {}
	virtual void LeaveScene() {}
	virtual void OnEnterScene() {}
	virtual void OnLeaveScene() {}
	virtual EObjType GetObjType() { return E_OBJ_INVALID; }
	virtual void IsEnemy(){}

	time_t LastTime() { return m_last; }
	time_t NowTime() { return m_now; }
	time_t GetDiffTime() { return m_now - m_last; }

	bool IsPlayer() { return this->GetObjType() == E_OBJ_PLAYER; }
	bool IsMonster() { return this->GetObjType() == E_OBJ_MONSTER; }
	bool IsPet() { return this->GetObjType() == E_OBJ_PET; }

	WorldPos* GetPos() { return &m_pos; }
	void SetWorldPos(const WorldPos& pos) { m_pos = pos; }
	Scene* GetScene() { return m_scene; }
	void SetScene(Scene* scene) { m_scene = scene; }
	int32_t GetZoneId() { return m_zoneid; }
	bool UpdateZone();

	void UpdateTime(time_t ti);

	void SendMsg(uint32_t msgid, ::google::protobuf::Message* msg);

public:
	int32_t m_objid;
protected:
	int32_t m_memid;
	WorldPos m_pos;
	Scene* m_scene;
	int32_t m_zoneid;
	float m_dir;  // 对象方向

	time_t m_last;
	time_t m_now;

	Context m_context;
};

