#pragma once
#include <string>
#include "NetProto/db/GameDB.pb.h"
using std::string;

class Table
{
public:
private:
};

// 对应数据库表
struct tb_account
{
	int id;
	string loginname;
	string loginpwd;
	string gameid;
	string openid;
	string token;
	string channelid;
	string whitelist;
	string snid;
	string mac;
};

struct tb_Role
{
	// 常用基础属性 
	uint64_t	role_id;	// 角色ID
	string	account;	// 账号
	string	role_name;	// 名称
	int32_t	role_sex;	// 性别
	int32_t	role_isvalid; // 角色是否有效
	int64_t	tm_login;	// 登录时间
	int64_t	tm_logout;	// 退出时间
	float	scene_id;
	float	pos_x;		// 
	float	pos_z;		//
	float	last_scene_id;	//
	float	last_pos_x;	// 
	float	last_pos_z;	//
	// 战斗属性数据
	int32_t	role_hp;
	int32_t	role_mp;
	// 各个模块数据
	string	activity_db;
	string	pet_db;
	string	tasks_db;
	string	skill_db;
	string	buff_db;
	string	resource_db;	// 角色资源（各种资源货币）
	string	his_resource_db;	// 角色资源（历史各种资源货币）
	string	item_db;
	string	equip_db;
};

void ConvertToPb(tb_Role& role_db, Role& role_pb, bool isjson = false);

void ConvertToTb(Role& role_pb, tb_Role& role_db, bool isjson = false);