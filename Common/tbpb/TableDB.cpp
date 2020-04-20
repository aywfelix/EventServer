#include "TableDB.h"
#include "PbJsonConvert.h"

void ConvertToPb(tb_Role& role_db, Role& role_pb, bool isjson)
{
	role_pb.Clear();
	role_pb.set_role_id(role_db.role_id);
	role_pb.set_account(role_db.account);
	role_pb.set_role_name(role_db.role_name);
	role_pb.set_role_sex(role_db.role_sex);
	role_pb.set_role_isvalid(role_db.role_isvalid);
	role_pb.set_tm_login(role_db.tm_login);
	role_pb.set_tm_logout(role_db.tm_logout);
	role_pb.set_scene_id(role_db.scene_id);
	role_pb.set_pos_x(role_db.pos_x);
	role_pb.set_pos_z(role_db.pos_z);
	role_pb.set_last_scene_id(role_db.last_scene_id);
	role_pb.set_last_pos_x(role_db.last_pos_x);
	role_pb.set_last_pos_z(role_db.last_pos_z);
	role_pb.set_role_hp(role_db.role_hp);
	role_pb.set_role_mp(role_db.role_mp);
	if (isjson)
	{
		PbJsonConvert::JsonStr2PbMsg(role_db.activity_db, *role_pb.mutable_activity_db(), true);
		PbJsonConvert::JsonStr2PbMsg(role_db.pet_db, *role_pb.mutable_pet_db(), true);
		PbJsonConvert::JsonStr2PbMsg(role_db.tasks_db, *role_pb.mutable_tasks_db(), true);
		PbJsonConvert::JsonStr2PbMsg(role_db.skill_db, *role_pb.mutable_skill_db(), true);
		PbJsonConvert::JsonStr2PbMsg(role_db.buff_db, *role_pb.mutable_buff_db(), true);
		PbJsonConvert::JsonStr2PbMsg(role_db.resource_db, *role_pb.mutable_resource_db(), true);
	}
	else
	{
		(role_pb.mutable_activity_db())->ParseFromString(role_db.activity_db);
		(role_pb.mutable_pet_db())->ParseFromString(role_db.pet_db);
		(role_pb.mutable_tasks_db())->ParseFromString(role_db.tasks_db);
		(role_pb.mutable_skill_db())->ParseFromString(role_db.skill_db);
		(role_pb.mutable_buff_db())->ParseFromString(role_db.buff_db);
		(role_pb.mutable_resource_db())->ParseFromString(role_db.resource_db);
	}
}

void ConvertToTb(Role& role_pb, tb_Role& role_db, bool isjson)
{
	role_db.role_id = role_pb.role_id();
	role_db.account = role_pb.account();
	role_db.role_name = role_pb.role_name();
	role_db.role_sex = role_pb.role_sex();
	role_db.role_isvalid = role_pb.role_isvalid();
	role_db.tm_login = role_pb.tm_login();
	role_db.tm_logout = role_pb.tm_logout();
	role_db.scene_id = role_pb.scene_id();
	role_db.pos_x = role_pb.pos_x();
	role_db.pos_z = role_pb.pos_z();
	role_db.last_scene_id = role_pb.last_scene_id();
	role_db.last_pos_x = role_pb.last_pos_x();
	role_db.last_pos_z = role_pb.last_pos_z();
	role_db.role_hp = role_pb.role_hp();
	role_db.role_mp = role_pb.role_mp();
	if (isjson)
	{
		PbJsonConvert::PbMsg2JsonStr(role_pb.activity_db(), role_db.activity_db, true);
		PbJsonConvert::PbMsg2JsonStr(role_pb.pet_db(), role_db.pet_db, true);
		PbJsonConvert::PbMsg2JsonStr(role_pb.tasks_db(), role_db.tasks_db, true);
		PbJsonConvert::PbMsg2JsonStr(role_pb.skill_db(), role_db.skill_db, true);
		PbJsonConvert::PbMsg2JsonStr(role_pb.buff_db(), role_db.buff_db, true);
		PbJsonConvert::PbMsg2JsonStr(role_pb.resource_db(), role_db.resource_db, true);
	}
	else
	{
		role_db.activity_db = role_pb.activity_db().SerializeAsString();
		role_db.pet_db = role_pb.pet_db().SerializeAsString();
		role_db.tasks_db = role_pb.tasks_db().SerializeAsString();
		role_db.skill_db = role_pb.skill_db().SerializeAsString();
		role_db.buff_db = role_pb.buff_db().SerializeAsString();
		role_db.resource_db = role_pb.resource_db().SerializeAsString();
	}
}