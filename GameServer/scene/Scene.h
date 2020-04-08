#pragma once
#include "map/ZoneMgr.h"

// ≥°æ∞¥¶¿Ì
class Scene
{
public:
	bool Init(int32_t mapxlen, int32_t mapzlen, int32_t zonesize);
	ZoneMgr GetZoneMgr() { return m_zonemgr; }
private:
	ZoneMgr m_zonemgr;
};

