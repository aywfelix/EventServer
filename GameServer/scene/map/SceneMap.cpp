#include "SceneMap.h"
#include "MapNav.h"
#include "scene/Scene.h"

bool SceneMap::Init(std::string mapfile)
{
	m_mapfile = mapfile;
	return true;
}

void SceneMap::Start()
{
	m_thread = std::thread(std::bind(&SceneMap::LoadMap, this));
}

void SceneMap::LoadMap()
{
	m_map_nav.LoadTile(m_mapfile);
}