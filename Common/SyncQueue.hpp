#pragma once

#include <list>
#include "Lock.hpp"

template<typename T>
class SyncQueue {
public:
	SyncQueue(int32_t max_size):m_max_size
	{
	}
	~SyncQueue()
	{}

	void Put(const T& t)
	{
		GuardLock lock(m_mutex);
		while (IsFull())
		{
			m_not_full.wait(m_mutex);
		}
		m_queue.emplace_back(t);
		m_not_empty.notify_one();
	}

	void Take(T& t)
	{
		GuardLock lock(m_mutex);
		while (IsEmpty())
		{
			m_not_empty.wait(m_mutex);
		}
		t = m_queue.front();
		m_queue.pop_front();
		m_not_full.notify_one();
	}

	bool Empty()
	{
		GuardLock lock(m_mutex);
		return m_queue.empty();
	}

	bool Full()
	{
		GuardLock lock(m_mutex);
		return m_queue.size() == m_max_size;
	}

	bool Size()
	{
		GuardLock lock(m_mutex);
		return m_queue.size();
	}

private:
	bool IsFull() const 
	{
		return m_queue.size() == m_max_size;
	}
	bool IsEmpty() const 
	{
		return m_queue.empty();
	}

private:
	std::list<T> m_queue;
	Mutex m_mutex;
	AnyCond m_not_empty;
	AnyCond m_not_full;
	int32_t m_max_size;
};
