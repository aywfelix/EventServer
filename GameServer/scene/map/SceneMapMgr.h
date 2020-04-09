#pragma once
#include <map>


class SceneMap;
class Scene;
class SceneMapMgr
{
	// <sceneid, <lineid, scenemap>>
	using scene_map_mgr_t = std::map<int32_t, std::map<int32_t, SceneMap*>>;

public:
	bool LoadAllMap();
	Scene* GetScene(int32_t mapid, int32_t lineid);
private:
	scene_map_mgr_t m_scene_map;
};

