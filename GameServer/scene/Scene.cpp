#include "Scene.h"
#include "map/SceneMap.h"

bool Scene::Init(int32_t mapxlen, int32_t mapzlen, int32_t zonesize)
{
	m_zone_mgr.Init(mapxlen, mapzlen, zonesize);
	return true;
}

