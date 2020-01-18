#pragma once

#include <map>
#include <memory>
#include "ConsistentHash.hpp"

template<typename T, typename TD>
class Map
{
	using map_t = std::map<T, TD*>;
public:
	Map(){}
	~Map(){}
	std::size_t size() { return m_map.size(); }
	bool is_empty() { return m_map.empty(); }
	void clear() { m_map.clear(); }
	void insert(const T& key, TD* data)
	{
		if (m_map.find(key) == m_map.end()) 
			m_map.emplace(key, data);
	}
	void erase(const T& key)
	{
		m_map.erase(key);
	}
	TD* get_value(const T& key)
	{
		auto iter = m_map.find(key);
		if (iter == m_map.end()) return nullptr;
		return iter->second;
	}
	TD* first()
	{
		if (m_map.size() == 0) return nullptr;
		m_iter = m_map.begin();
		if (m_iter == m_map.end()) return nullptr;
		return m_iter->second;
	}
	TD* next()
	{
		m_iter++;
		if (m_iter == m_map.end()) return nullptr;
		return m_iter->second;
	}
	TD* first(T& key)
	{
		if (m_map.size() == 0) return nullptr;
		m_iter = m_map.begin();
		if (m_iter == m_map.end()) return nullptr;
		key = m_iter->first;
		return m_iter->second;
	}
	TD* next(T& key)
	{
		m_iter++;
		if (m_iter == m_map.end()) return nullptr;
		key = m_iter->first;
		return m_iter->second;
	}
private:
	map_t m_map;
	typename map_t::iterator m_iter;
};

template<typename T, typename TD>
class MapEx
{
	typedef std::shared_ptr<TD> TD_PTR;
	typedef std::map<T, TD_PTR> mapex_t;
public:
	MapEx() {}
	~MapEx() {}
	std::size_t size() { return m_mapex.size(); }
	bool is_empty() { return m_mapex.empty(); }
	void clear() { m_mapex.clear(); }
	void insert(const T& key, const TD& data)
	{
		TD_PTR TD_PTR = std::make_shared<TD>(data);
		if (m_mapex.find(key) == m_mapex.end()) m_mapex.emplace(key, TD_PTR);
	}
	void insert(const T& key, TD_PTR& TD_PTR)
	{
		if (m_mapex.find(key) == m_mapex.end()) m_mapex.emplace(key, TD_PTR);
	}
	void erase(const T& key)
	{
		m_mapex.erase(key);
	}
	void get_value(const T& key, TD& data)
	{
		if (m_mapex.find(key) == m_mapex.end()) return;
		data = m_mapex[key];
	}
	void get_value(const T& key, TD_PTR& TD_PTR)
	{
		if (m_mapex.find(key) == m_mapex.end()) return;
		TD_PTR = TD_PTR(m_mapex[key]);
	}
	TD* first_nude()
	{
		if (m_mapex.size() == 0) return nullptr;
		m_iter = m_mapex.begin();
		if (m_iter == m_mapex.end()) return nullptr;
		return (m_iter->second).get();
	}
	TD_PTR first()
	{
		if (m_mapex.size() == 0) return nullptr;
		m_iter = m_mapex.begin();
		if (m_iter == m_mapex.end()) return nullptr;
		return m_iter->second;
	}
	TD* next_nude()
	{
		m_iter++;
		if (m_iter == m_mapex.end()) return nullptr;
		return (m_iter->second).get();
	}
	TD_PTR next()
	{
		m_iter++;
		if (m_iter == m_mapex.end()) return nullptr;
		return m_iter->second;
	}
	TD* first_nude(T& key)
	{
		if (m_mapex.size() == 0) return nullptr;
		m_iter = m_mapex.begin();
		if (m_iter == m_mapex.end()) return nullptr;
		key = m_iter->first;
		return (m_iter->second).get();
	}
	TD_PTR first(T& key)
	{
		if (m_mapex.size() == 0) return nullptr;
		m_iter = m_mapex.begin();
		if (m_iter == m_mapex.end()) return nullptr;
		key = m_iter->first;
		return m_iter->second;
	}
	TD* next_nude(T& key)
	{
		m_iter++;
		if (m_iter == m_mapex.end()) return nullptr;
		key = m_iter->first;
		return (m_iter->second).get();
	}
	TD_PTR next(T& key)
	{
		m_iter++;
		if (m_iter == m_mapex.end()) return nullptr;
		key = m_iter->first;
		return m_iter->second;
	}

protected:
	mapex_t m_mapex;
	typename mapex_t::iterator m_iter;
};


template<typename T, typename TD>
class ConsistentHashEx : public MapEx<T, TD>
{
public:
	void insert(const T& name, TD& data)
	{
		auto iter = MapEx<T, TD>::m_mapex.find(name);
		if (iter == MapEx<T, TD>::m_mapex.end())
		{
			MapEx<T, TD>::m_mapex.emplace(name, std::make_shared<TD>(data));
			m_consistenthash.insert(name);
		}
	}
	void insert(const T& name, std::shared_ptr<TD>& data)
	{
		auto iter = MapEx<T, TD>::m_mapex.find(name);
		if (iter == MapEx<T, TD>::m_mapex.end())
		{
			MapEx<T, TD>::m_mapex.emplace(name, data);
			m_consistenthash.insert(name);
		}
	}
	void remove(const T& name)
	{
		auto iter = MapEx<T, TD>::m_mapex.find(name);
		if (iter != MapEx<T, TD>::m_mapex.end())
		{
			MapEx<T, TD>::m_mapex.erase(iter);
			m_consistenthash.erase(name);
		}
	}

	std::shared_ptr<TD> get_server_random()
	{
		VirtualNode<T> vnode;
		if (m_consistenthash.get_random_node(vnode))
		{
			auto iter = MapEx<T, TD>::m_mapex.find(vnode.m_data);
			if (iter != MapEx<T, TD>::m_mapex.end())
			{
				return iter->second;
			}
		}
		return nullptr;
	}
	std::shared_ptr<TD> get_server_consistent()
	{
		VirtualNode<T> vnode;
		if (m_consistenthash.get_node(vnode))
		{
			auto iter = MapEx<T, TD>::m_mapex.find(vnode.m_data);
			if (iter != MapEx<T, TD>::m_mapex.end())
			{
				return iter->second;
			}
		}
		return nullptr;
	}
	std::shared_ptr<TD> get_server_by_key(const T& name)
	{
		VirtualNode<T> vnode;
		if (m_consistenthash.get_node(name, vnode))
		{
			auto iter = MapEx<T, TD>::m_mapex.find(vnode.m_data);
			if (iter != MapEx<T, TD>::m_mapex.end())
			{
				return iter->second;
			}
		}
		return nullptr;
	}
	void clear()
	{
		MapEx<T, TD>::m_mapex.clear();
		m_consistenthash.clear();
	}
private:
	ConsistentHash<T> m_consistenthash;
};