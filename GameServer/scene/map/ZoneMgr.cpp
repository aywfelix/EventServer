#include "ZoneMgr.h"
#include "obj/ObjPlayer.h"


std::unique_ptr<ZoneMgr> g_pZoneMgr = nullptr;

bool ZoneMgr::Init(int32_t mapxlen, int32_t mapzlen, int32_t zonesize)
{
	m_zonesize = zonesize;

	m_zoneinfo.m_mapxlen = mapxlen;
	m_zoneinfo.m_mapzlen = mapzlen;

	int xsize = mapxlen / zonesize;
	if (mapxlen % zonesize > 0) xsize += 1;
	int zsize = mapzlen / zonesize;
	if (mapzlen % zonesize > 0) zsize += 1;

	m_zoneinfo.m_xsize = xsize;
	m_zoneinfo.m_zsize = zsize;
	m_zoneinfo.m_size = xsize * zsize;

	m_zones.resize(m_zoneinfo.m_size);

	for (int32_t i = 0; i < m_zoneinfo.m_size; i++)
	{
		int32_t x = i % m_zoneinfo.m_xsize;
		int32_t z = i / m_zoneinfo.m_xsize;
		WorldPos pos;
		pos.m_x = float(x * m_zonesize + m_zonesize / 2);
		pos.m_z = float(z * m_zonesize + m_zonesize / 2);

		m_zones[i].SetCenter(pos);
		m_zones[i].SetZoneId(i);
		m_zones[i].SetZoneSize(m_zonesize);
	}
	return true;
}

void ZoneMgr::Clear()
{
	m_zones.clear();
}

bool ZoneMgr::EnterZone(Obj* obj, int32_t zoneid, EZoneRange range)
{
	m_zones[zoneid].EnterZone(obj);
	set_player_t players = GetZonePlayerList(zoneid, range);
	SendMsgEnter(obj, players);
	return true;
}
bool ZoneMgr::LeaveZone(Obj* obj, int32_t zoneid, EZoneRange range)
{
	m_zones[zoneid].LeaveZone(obj);
	set_player_t players = GetZonePlayerList(zoneid, range);
	SendMsgLeave(obj, players);
	return true;
}
bool ZoneMgr::ChangeZone(Obj* obj, int32_t oldzoneid, int32_t newzoneid, EZoneRange range)
{
	zone_ids_t oldzoneids, newzoneids;
	GetChangeZoneIdList(oldzoneid, newzoneid, oldzoneids, newzoneids, range);
	for (auto zoneid : oldzoneids)
	{
		LeaveZone(obj, zoneid, range);
	}

	for (auto zoneid : oldzoneids)
	{
		EnterZone(obj, zoneid, range);
	}
	return true;
}

ZoneRect ZoneMgr::GetZoneRect(int32_t zoneid, EZoneRange range)
{
	ZoneRect rect;
	int x = zoneid % m_zoneinfo.m_xsize;
	int z = zoneid / m_zoneinfo.m_xsize;
	rect.sx = x - range;
	if (rect.sx < 0) rect.sx = 0;
	rect.ex = x + range;
	if (rect.ex >= m_zoneinfo.m_xsize) rect.ex = m_zoneinfo.m_xsize - 1;
	rect.sz = z - range;
	if (rect.sz < 0) rect.sz = 0;
	rect.ez = z + range;
	if (rect.ez >= m_zoneinfo.m_zsize) rect.ez = m_zoneinfo.m_zsize - 1;
	return rect;
}

zone_ids_t ZoneMgr::GetZoneIdList(int32_t zoneid, EZoneRange range)
{
	zone_ids_t ids;
	ZoneRect rect = GetZoneRect(zoneid, range);
	for (int x = rect.sx; x <= rect.ez; x++)
	{
		for (int z = rect.sz; z <= rect.ez; z++)
		{
			int32_t zoneid = x + z * m_zoneinfo.m_xsize;
			if (zoneid >= 0 && zoneid < m_zoneinfo.m_size)
			{
				ids.push_back(zoneid);
			}
		}
	}
	return ids;
}

void ZoneMgr::GetChangeZoneIdList(int32_t oldzoneid, int32_t newzoneid, zone_ids_t& oldzoneids, zone_ids_t& newzoneids, EZoneRange range)
{
	oldzoneids.clear();
	newzoneids.clear();
	ZoneRect oldrect = GetZoneRect(oldzoneid, range);
	ZoneRect newrect = GetZoneRect(oldzoneid, range);
	for (int x = oldrect.sx; x <= oldrect.ez; x++)
	{
		for (int z = oldrect.sz; z <= oldrect.ez; z++)
		{
			if (!newrect.IsInRect(x, z))
			{
				int32_t zoneid = x + z * m_zoneinfo.m_xsize;
				if (zoneid >= 0 && zoneid < m_zoneinfo.m_size)
				{
					oldzoneids.push_back(zoneid);
				}
			}
		}
	}
	
	for (int x = newrect.sx; x <= newrect.ez; x++)
	{
		for (int z = newrect.sz; z <= newrect.ez; z++)
		{
			if (!oldrect.IsInRect(x, z))
			{
				int32_t zoneid = x + z * m_zoneinfo.m_xsize;
				if (zoneid >= 0 && zoneid < m_zoneinfo.m_size)
				{
					newzoneids.push_back(zoneid);
				}
			}
		}
	}
}

set_player_t ZoneMgr::GetZonePlayerList(int32_t zoneid, EZoneRange range)
{
	zone_ids_t zoneids;
	zone_ids_t ids;
	if (range == E_ZONE_ZERO)
	{
		zoneids.emplace_back(zoneid);
	}
	if (range > E_ZONE_ZERO)
	{
		ids = GetZoneIdList(zoneid, range);
		zoneids.assign(ids.begin(), ids.end());
	}
	set_player_t players;
	for (auto i : zoneids)
	{
		set_player_t& set_players = m_zones[i].GetPlayers();
		for (auto& it : set_players)
		{
			players.emplace(it);
		}
	}
	return players;
}

void ZoneMgr::SendMsgEnter(Obj* obj, set_player_t& players)
{
	SendMsgToMe(obj, players);
	SendMsgToOthers(obj, players);
}

void ZoneMgr::SendMsgLeave(Obj* obj, set_player_t& players)
{
	SendMsgToMe(obj, players);
	SendMsgToOthers(obj, players);
}

void ZoneMgr::SendMsgToMe(Obj* obj, set_player_t& players)
{
	for (auto& player : players)
	{
		if (player != obj)
		{
			// 构造其他人信息
		}
	}
	uint32_t msgid = 1;
	::google::protobuf::Message* msg = nullptr;
	obj->SendMsg(msgid, msg);
}

void ZoneMgr::SendMsgToOthers(Obj* obj, set_player_t& players)
{
	for (auto& player : players)
	{
		if (player != obj)
		{
			uint32_t msgid = 1;
			::google::protobuf::Message* msg = nullptr;
			obj->SendMsg(msgid, msg);
		}
	}
}
