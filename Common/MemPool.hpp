#pragma once
#pragma once

#include <list>
#include <map>
#include "Lock.hpp"

class MemElem
{
public:
	virtual ~MemElem(){}
	virtual void SetMemId(int memid)
	{
		m_memid = memid;
	}
	virtual int GetMemId()
	{
		return m_memid;
	}
	virtual bool Init() { return true; }
	virtual void Clear() {  }
private:
	int m_memid{ 0 };
};

template <typename T>
class MemPool
{
public:
	MemPool()
	{
		m_list.clear();
		m_use.clear();
		m_incr = 50;  // default increment 50 TODO
		m_index = 0;
		m_count = 0;

		Expand();
	}

	static MemPool<T>* Instance()
	{
		static MemPool<T> pool;
		return &pool;
	}

	virtual ~MemPool()
	{
		for (auto& it : m_list)
		{
			delete it;
		}
		for (auto& it : m_use)
		{
			delete it.second;
		}
		m_list.clear();
		m_use.clear();
	}

	T* NewElem()
	{
		GuardLock lock(m_mutex);
		if (m_list.size() <= 0)
		{
			Expand();
		}
		if (m_list.size() > 0)
		{
			T* t = m_list.front();
			m_use.emplace(t->GetMemId(), t);
			m_list.pop_front();
			return t;
		}
		return nullptr;
	}

	T* GetElem(int id)
	{
		auto it = m_use.find(id);
		if (it == m_use.end())
		{
			return nullptr;
		}
		return it->second;
	}

	bool DelElem(int id)
	{
		GuardLock lock(m_mutex);
		auto it = m_use.find(id);
		if (it == m_use.end())
		{
			return false;
		}

		T* t = it->second;
		t->Clear();
		m_list.push_back(t);
		m_use.erase(it);
		return true;
	}

private:
	bool Expand()
	{
		for (int i = 0; i < m_incr; ++i)
		{
			T* t = new T();
			if (t == nullptr)
			{
				return false;
			}
			m_list.push_back(t);
			t->SetMemId(m_index++);
		}
		m_count += m_incr;
		return true;
	}

private:
	std::list<T*> m_list;
	std::map<int, T*> m_use;
	int m_incr;
	std::atomic<int> m_index;
	Mutex m_mutex;
	int m_count;
};