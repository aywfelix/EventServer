#include "Scene.h"
#include <functional>

Scene::Scene(SceneImpl* impl)
{
	m_sceneimpl = impl;
}
Scene::~Scene()
{

}

bool Scene::Init(int32_t mapxlen, int32_t mapzlen, int32_t zonesize)
{
	m_zone_mgr.Init(mapxlen, mapzlen, zonesize);
	return true;
}

void Scene::Start(const std::string& mapfile)
{
	m_thread = std::thread(std::bind(&Scene::LoadMap, this, mapfile));
}

void Scene::LoadMap(const std::string& mapfile)
{
	m_map_nav.LoadTile(mapfile);
}
