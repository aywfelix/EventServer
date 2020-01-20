#pragma once

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "jsoncpp/json.h"
#include "easylogging++.h"
#include "Config.h"

class ActivityRow
{
public:
	int id;                         //key
	std::vector<int> item_list;     //道具id	点击活动使用的道具id
};

class ActivityTable
{
private:
	typedef std::unique_ptr<ActivityRow> ptr_row_type;
	typedef std::unordered_map<int, ptr_row_type> map_table_type;
	typedef std::vector<int> vec_type;

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

	void Load()
	{
		LoadJson("Activity.json");
	}

	void ReLoad()
	{
		ReLoadJson("Activity.json");
	}

	void LoadJson(const std::string& jsonFile)
	{
		g_pConfig->Load(jsonFile.c_str());

		for (auto it = g_pConfig->m_Root.begin(); it != g_pConfig->m_Root.end(); ++it)
		{
			try
			{
				auto& r = (*it);
				ptr_row_type pRow(new ActivityRow);
				ActivityRow& row = *pRow;
				row.id = r["id"].asInt();

				auto end_item_list = r["item_list"].end();
				auto begin_item_list = r["item_list"].end();
				for (auto it = begin_item_list; it != end_item_list; ++it)
				{
					row.item_list.emplace_back(it->asInt());
				}
				
				m_table.emplace(row.id, pRow);
				m_keys.emplace_back(row.id);
			}
			catch (std::exception const& e)
			{
				LOG(ERROR) << "read table Activity error," << e.what() << ":" << (*it)["id"].asInt();
				exit(-1);
			}
		}
	}

	bool ReLoadJson(const std::string& jsonFile)
	{
		m_keys.clear();
		m_table.clear();
		LoadJson(jsonFile);
	}

};