#pragma once
#include "map/ZoneMgr.h"
#include "map/SceneMap.h"
#include "common/GameDef.h"

class SceneMap;
class SceneImpl;

// ≥°æ∞¥¶¿Ì
class Scene
{
public:
	Scene(){}
	Scene(SceneImpl* impl);
	virtual ~Scene();

	bool Init(int32_t mapxlen, int32_t mapzlen, int32_t zonesize);

	ESceneType GetSceneType() { return m_scene_type; }
	void SetSceneMap(SceneMap* scenemap) { m_scene_map = scenemap; }
	SceneMap* GetSceneMap() { return m_scene_map; }

	virtual bool ObjEnterScene(Obj* obj) { return true; }
	virtual bool ObjLeaveScene(Obj* obj) { return true; }
	virtual bool LoadScene(Obj* obj) { return true; }
protected:
	ESceneType m_scene_type;
	SceneMap* m_scene_map;
	ZoneMgr m_zone_mgr;
	SceneImpl* m_sceneimpl;
};

