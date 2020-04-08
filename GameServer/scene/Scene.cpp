#include "Scene.h"

bool Scene::Init(int32_t mapxlen, int32_t mapzlen, int32_t zonesize)
{
	m_zonemgr.Init(mapxlen, mapzlen, zonesize);
	return true;
}