#pragma once
#include "ZoneMgr.h"
#include "common/GameDef.h"
#include <string>
#include <memory>
#include <thread>
#include "MapNav.h"

class SceneImpl;
// ≥°æ∞¥¶¿Ì
class Scene
{
public:
	Scene(){}
	Scene(SceneImpl* impl);
	virtual ~Scene();

	bool Init(int32_t mapxlen, int32_t mapzlen, int32_t zonesize);

	ESceneType GetSceneType() { return E_SCENE_VOID; }

	virtual bool ObjEnterScene(Obj* obj) { return true; }
	virtual bool ObjLeaveScene(Obj* obj) { return true; }
	virtual bool LoadScene(Obj* obj) { return true; }
	virtual void SpawnMonster() {}

	void Start(const std::string& mapfile);
	MapNav* GetMapNav() { return &m_map_nav; }
private:
	void LoadMap(const std::string& mapfile);
protected:
	//scenemap_t m_scene_map;
	ZoneMgr m_zone_mgr;
	SceneImpl* m_sceneimpl;

	std::thread m_thread;
	MapNav m_map_nav;
};

