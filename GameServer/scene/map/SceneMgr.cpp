#include "SceneMgr.h"
#include "Scene.h"
#include "datatable/Scene.hpp"
#include "scene/gmscene/GameScene.h"
#include "scene/mirror/MirrorScene.h"
#include "scene/dungeon/FightScene.h"

std::unique_ptr<SceneMgr> g_pSceneMgr = nullptr;

bool SceneMgr::LoadAllMap()
{
	auto& scene_table = SceneTable::Instance()->table();
	for (auto it = scene_table.begin(); it != scene_table.end(); it++)
	{
		int scene_id = it->first;
		auto scene_row = it->second;
		int lines = scene_row->lines;
		line_scene_t line_scene;
		for (int i = 1; i <= lines; i++)
		{
			scene_t scene = std::make_shared<Scene>();
			scene->Init(1000, 1000, 20);
			scene->Start(scene_row->path);
			line_scene.emplace(i, scene);
		}
		m_scene_mgr.emplace(scene_id, line_scene);
	}
	return true;
}

Scene* SceneMgr::GetScene(int32_t sceneid, int32_t lineid)
{
	auto scene_mgr_it = m_scene_mgr.find(sceneid);
	if (scene_mgr_it == m_scene_mgr.end()) 
		return nullptr;
	line_scene_t line_scene = scene_mgr_it->second;
	auto scene_it = line_scene.find(lineid);
	if (scene_it == line_scene.end()) 
		return nullptr;
	Scene* scene = (scene_it->second).get();
	return scene;
}

Scene* SceneMgr::CreatTmpScene(uint64_t playerid, ESceneType type)
{
	auto it = m_tmp_scene.find(playerid);
	if (it != m_tmp_scene.end()) 
		return nullptr;

	scene_t scene = nullptr;
	switch (type)
	{
	case E_SCENE_NORM:
		scene = std::shared_ptr<Scene>(new GameScene);
		break;
	case E_SCENE_MIRROR:
		scene = std::shared_ptr<Scene>(new MirrorScene);
		break;
	case E_SCENE_DUNGEON:
		scene = std::shared_ptr<Scene>(new FightScene(nullptr));
		break;
	default:
		break;
	}
	if (scene)
	{
		m_tmp_scene.emplace(playerid, scene);
		return scene.get();
	}
	return nullptr;
}

Scene* SceneMgr::GetTmpScene(uint64_t playerid)
{
	auto it = m_tmp_scene.find(playerid);
	if (it == m_tmp_scene.end())
	{
		return nullptr;
	}
	return it->second.get();
}

void SceneMgr::DelTmpScene(uint64_t playerid)
{
	auto it = m_tmp_scene.find(playerid);
	if (it == m_tmp_scene.end())  return;
	m_tmp_scene.erase(playerid);
}