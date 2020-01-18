#pragma once

#include <string>
#include <sstream>
#include <stdint.h>
#include <map>
#include <chrono>
#include "lexical_cast.hpp"
#include "cityhash/city.h"

template<typename T>
class VirtualNode
{
public:
	VirtualNode(const T data, const int vid):m_data(data), m_vid(vid){}
	VirtualNode(){}
	~VirtualNode(){}
	std::string tostr()
	{
		std::ostringstream oss;
		oss << lexical_cast<std::string>(m_data) << "-" << m_vid;
		return oss.str();
	}

	T m_data;
	int m_vid;
};

template<typename T>
class ConsistentHash
{
	using map_t = std::map<uint32_t, VirtualNode<T>>;
	using vir_node_t = VirtualNode<T>;
public:
	ConsistentHash() {}
	~ConsistentHash() {}
	bool is_empty() { return m_nodes.empty(); }
	std::size_t size() { return m_nodes.size(); }
	void clear() { m_nodes.clear(); }
	void insert(const T& name)
	{
		for (int i = 0; i < m_node_count; ++i)
		{
			VirtualNode<T> vnode(name, i);
			insert(vnode);
		}
	}
	void insert(VirtualNode<T>& vnode)
	{
		uint32_t hash = get_hash(vnode);
		if (m_nodes.find(hash) == m_nodes.end()) 
			m_nodes.emplace(hash, vnode);
	}

	bool exist(VirtualNode<T>& vnode)
	{
		uint32_t hash = get_hash(vnode);
		if (m_nodes.find(hash) == m_nodes.end()) return false;
		return true;
	}
	void erase(VirtualNode<T>& vnode)
	{
		uint32_t hash = get_hash(vnode);
		if (m_nodes.find(hash) == m_nodes.end()) return;
		m_nodes.erase(hash);
	}
	void erase(const T& name)
	{
		for (int i = 0; i < m_node_count; ++i)
		{
			VirtualNode<T> vnode(name, i);
			erase(vnode);
		}
	}
	bool get_random_node(VirtualNode<T>& vnode)
	{
		int vid = (int)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		return get_node(vid, vnode);
	}
	bool get_node(VirtualNode<T>& vnode)
	{
		return get_node(0, vnode);
	}
	bool get_node(const T& name, VirtualNode<T>& vnode)
	{
		std::string str = lexical_cast<std::string>(name);
		uint32_t hash = CityHash32(str.c_str(), str.length());
		return get_node(hash, vnode);
	}

private:
	bool get_node(uint32_t hash, VirtualNode<T>& vnode)
	{
		if (m_nodes.empty()) return false;
		auto it = m_nodes.lower_bound(hash);
		if (it == m_nodes.end()) it = m_nodes.begin();
		vnode = it->second;
		return true;
	}
	uint32_t get_hash(VirtualNode<T>& node)
	{
		std::string node_str = node.tostr();
		uint32_t hash = CityHash32(node_str.c_str(), node_str.length());
		return hash;
	}
private:
	int m_node_count = 100;
	map_t m_nodes;
};