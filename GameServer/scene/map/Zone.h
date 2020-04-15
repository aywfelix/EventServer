#pragma once
#include "common/Common.h"
#include <set>

class Obj;
class ObjMonster;
class ObjPlayer;
class ::google::protobuf::Message;

using set_obj_t = std::set<Obj*>;
using set_player_t = std::set<Obj*>;
using set_monster_t = std::set<Obj*>;
// µØÍ¼¸ñ×Ó
class Zone
{
public:
	Zone();
	~Zone();
	void SetCenter(WorldPos& pos) { m_center = pos; }
	WorldPos GetCenter() { return m_center; }
	void SetZoneSize(int32_t size) { m_zonesize = size; }
	int32_t GetZoneSize() { return m_zonesize; }
	void SetZoneId(int zoneid) { m_zoneid = zoneid; }
	int32_t GetZoneId() { return m_zoneid; }

	float XMin();
	float ZMin();
	float XMax();
	float ZMax();

	void EnterZone(Obj* obj);
	void LeaveZone(Obj* obj);
	set_obj_t& GetObjs() { return m_objs; }
	set_player_t& GetPlayers() { return m_players; }
	set_monster_t& GetMonsters() { return m_monsters; }

	void ZoneSendMsg(uint32_t msgid, ::google::protobuf::Message* msg);


private:
	WorldPos m_center;
	int32_t m_zonesize;
	int32_t m_zoneid;

	set_obj_t m_objs;
	set_player_t m_players;
	set_monster_t m_monsters;
};

