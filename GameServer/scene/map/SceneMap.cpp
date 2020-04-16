#include "SceneMap.h"
#include "MapNav.h"
#include "scene/Scene.h"


void SceneMap::Start(const std::string& mapfile)
{
	m_thread = std::thread(std::bind(&SceneMap::LoadMap, this, mapfile));
}

void SceneMap::LoadMap(const std::string& mapfile)
{
	m_map_nav.LoadTile(m_mapfile);
}