#pragma once
#include <string>

class Table
{
public:
private:
};

// 对应数据库表
struct AccountTB
{
	int id;
	std::string loginname;
	std::string loginpwd;
	std::string gameid;
	std::string openid;
	std::string token;
	std::string channelid;
	std::string whitelist;
	std::string snid;
	std::string mac;
};

struct PlayerTB
{
	std::string playerid;
	std::string gameid;
	int roleid;
	std::string nickname;
	int sex;
	int level;
	std::string create_tm;
	std::string online_tm;
	std::string offline_tm;
	int gold;
	std::string guildid;
};