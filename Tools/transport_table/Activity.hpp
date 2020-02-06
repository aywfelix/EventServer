#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "ServerComm/CommonDef.h"
#include "jsoncpp/json.h"
#include "easylogging/easylogging++.h"
#include "Config.h"

class ActivityRow
{
public:
	int id;                                           //主键id 活动id
std::string comment;                              //注释 
int tab_id;                                       //标签编号 活动所属标签
int weight;                                       //权重 标签列表内的显示权重
int name_id;                                      //名称id 活动名称id
int describe_id;                                  //描述文本id 
int race_id;                                      //是否相关种族 是否需要判断玩家种族来显示对话按钮,0表示否，1表示是
std::string main_icon;                            //主图名称 
std::string sub_icon;                             //副图名称 
int function_on;                                  //是否开启 0 不开启 1 开启
int recommend;                                    //是否开启推荐 0 不开启 1 开启
int call;                                         //支持开启弹窗提醒 0 不开启 1 开启
int push;                                         //支持开启推送提醒 0 不开启 1 开启
int open_type;                                    //时间类型 1 全天开放 2 限时开放
std::vector<int> open_in_week;                    //开放周次 限时开放时使用 （例）2|4
std::vector<int> open_in_day;                     //开放时间 限时开放时使用 （例 00:00 - 23:59） 0|0|23|59
int reset_type;                                   //重置次数类型 1 每天 2 每周
int level_min;                                    //最低等级限制 
int player_min;                                   //最低组队玩家数量 
int active_value;                                 //活跃度奖励 完成每轮任务奖励的活跃度
int rounds_max;                                   //轮次数上限 
int times_max;                                    //环次数上限 总=环×轮
std::vector<int> drop_rounds;                     //每轮掉落奖励 计算方式|掉落包id （掉落计算：0概率；1权值）
std::vector<int> drop_times;                      //每环掉落奖励 计算方式|掉落包id （掉落计算：0概率；1权值）
std::vector<int> drop_display;                    //奖励显示 
std::vector<int> scene_id;                        //场景id 活动涉及的所有场景id
int quest_id;                                     //记录任务id 记录活跃度的任务id

};

class ActivityTable
{
	typedef std::shared_ptr<ActivityRow> ptr_row_type;
	typedef std::unordered_map<int, ptr_row_type> map_table_type;
	typedef std::vector<int> vec_type;	
private:
	vec_type m_keys;
	map_table_type	m_table;
public:
	static ActivityTable* Instance()
	{
		static ActivityTable instance;
		return &instance;
	}

	const ActivityRow* GetRow(int key)
	{
		map_table_type::iterator it = m_table.find(key);
		if (it == m_table.end())
		{
			return nullptr;
		}
		return it->second.get();
	}

	bool HasRow(int key)
	{
		return m_table.find(key) != m_table.end();
	}

	const vec_type& Keys() const
	{
		return m_keys;
	}

	const map_table_type& table() const
	{
		return m_table;
	}

	bool Load()
	{
		return LoadJson("Activity.json");
	}

	bool ReLoad()
	{
		return ReLoadJson("Activity.json");
	}

	bool LoadJson(const std::string& jsonFile)
	{
		std::string loadfile = std::string(TABLE_PATH).append(jsonFile.c_str());
		if (!g_pConfig->Load(loadfile.c_str()))
		{
			LOG(ERROR) << "load table Activity error";
			return false;
		}

		for (auto it = g_pConfig->m_Root.begin(); it != g_pConfig->m_Root.end(); ++it)
		{
			try
			{
				auto& r = (*it);
				ptr_row_type pRow(new ActivityRow);
				ActivityRow& row = *pRow;
				                row.id = r["id"].asInt();
                row.comment = r["comment"].asString();
                row.tab_id = r["tab_id"].asInt();
                row.weight = r["weight"].asInt();
                row.name_id = r["name_id"].asInt();
                row.describe_id = r["describe_id"].asInt();
                row.race_id = r["race_id"].asInt();
                row.main_icon = r["main_icon"].asString();
                row.sub_icon = r["sub_icon"].asString();
                row.function_on = r["function_on"].asInt();
                row.recommend = r["recommend"].asInt();
                row.call = r["call"].asInt();
                row.push = r["push"].asInt();
                row.open_type = r["open_type"].asInt();

                auto end_open_in_week = r["open_in_week"].end();
				auto begin_open_in_week = r["open_in_week"].end();
				for (auto it = begin_open_in_week; it != end_open_in_week; ++it)
				{
					row.open_in_week.emplace_back(it->asInt());
				}
            
                auto end_open_in_day = r["open_in_day"].end();
				auto begin_open_in_day = r["open_in_day"].end();
				for (auto it = begin_open_in_day; it != end_open_in_day; ++it)
				{
					row.open_in_day.emplace_back(it->asInt());
				}
                            row.reset_type = r["reset_type"].asInt();
                row.level_min = r["level_min"].asInt();
                row.player_min = r["player_min"].asInt();
                row.active_value = r["active_value"].asInt();
                row.rounds_max = r["rounds_max"].asInt();
                row.times_max = r["times_max"].asInt();

                auto end_drop_rounds = r["drop_rounds"].end();
				auto begin_drop_rounds = r["drop_rounds"].end();
				for (auto it = begin_drop_rounds; it != end_drop_rounds; ++it)
				{
					row.drop_rounds.emplace_back(it->asInt());
				}
            
                auto end_drop_times = r["drop_times"].end();
				auto begin_drop_times = r["drop_times"].end();
				for (auto it = begin_drop_times; it != end_drop_times; ++it)
				{
					row.drop_times.emplace_back(it->asInt());
				}
            
                auto end_drop_display = r["drop_display"].end();
				auto begin_drop_display = r["drop_display"].end();
				for (auto it = begin_drop_display; it != end_drop_display; ++it)
				{
					row.drop_display.emplace_back(it->asInt());
				}
            
                auto end_scene_id = r["scene_id"].end();
				auto begin_scene_id = r["scene_id"].end();
				for (auto it = begin_scene_id; it != end_scene_id; ++it)
				{
					row.scene_id.emplace_back(it->asInt());
				}
                            row.quest_id = r["quest_id"].asInt();

				m_table.emplace(row.id, pRow);
				m_keys.emplace_back(row.id);
			}
			catch (std::exception const& e)
			{
				LOG(ERROR) << "read table Activity error," << e.what() << ":" << (*it)["id"].asInt();
				return false;
			}
		}
		return true;
	}

	bool ReLoadJson(const std::string& jsonFile)
	{
		m_keys.clear();
		m_table.clear();
		return LoadJson(jsonFile);
	}

};