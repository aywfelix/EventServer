#include "Zone.h"
#include "obj/ObjMonster.h"
#include "obj/ObjPlayer.h"
#include "google/protobuf/message.h"
#include <algorithm>


Zone::Zone()
{

}

Zone::~Zone()
{

}

float Zone::XMin()
{
	return (m_center.m_x - m_zonesize / 2);
}
float Zone::ZMin()
{
	return (m_center.m_z - m_zonesize / 2);
}
float Zone::XMax()
{
	return (m_center.m_x + m_zonesize / 2);
}
float Zone::ZMax()
{
	return (m_center.m_z + m_zonesize / 2);
}

void Zone::EnterZone(Obj* obj)
{
	switch (obj->GetObjType())
	{
	case E_OBJ_INVALID:
		return;
	case E_OBJ_PLAYER:
		m_players.emplace(obj);
		break;
	case E_OBJ_MONSTER:
		m_monsters.emplace(obj);
		break;
	default:
		m_objs.emplace(obj);
		break;
	}
}

void Zone::LeaveZone(Obj* obj)
{
	switch (obj->GetObjType())
	{
	case E_OBJ_INVALID:
		return;
	case E_OBJ_PLAYER:
		m_players.erase(obj);
		break;
	case E_OBJ_MONSTER:
		m_monsters.erase(obj);
		break;
	default:
		m_objs.erase(obj);
		break;
	}
}

void Zone::ZoneSendMsg(uint32_t msgid, ::google::protobuf::Message* msg)
{
	for (auto it : m_players)
	{
		it->SendMsg(msgid, msg);
	}
}