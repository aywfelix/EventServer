#pragma once

#include <list>
#include "Lock.hpp"

template<typename T>
class SyncQueue {
public:
	SyncQueue(int32_t max_size):m_max_size(max_size),m_need_stop(false)
	{
	}
	SyncQueue() :m_max_size(1000000), m_need_stop(false) 
	{}
	~SyncQueue()
	{}

	void Put(T& t)
	{
		Add(std::forward<T>(t));
	}

	void Put(T&& t)
	{
		Add(std::forward<T>(t));
	}

	void Take(T& t)
	{
		UniqueLock lock(m_mutex);
		m_not_empty.wait(lock, [this]() {return m_need_stop || !IsEmpty(); });
		if (m_need_stop)
		{
			return;
		}
		t = m_queue.front();
		m_queue.pop_front();
		m_not_full.notify_one();
	}

	void Take(std::list<T>& list)
	{
		UniqueLock lock(m_mutex);
		m_not_empty.wait(lock, [this]() {return m_need_stop || !IsEmpty(); });
		if (m_need_stop)
		{
			return;
		}
		list = std::move(m_queue);
		m_not_full.notify_one();
	}

	bool IsFull() const
	{
		return m_queue.size() == m_max_size;
	}
	bool IsEmpty() const
	{
		return m_queue.empty();
	}
	bool Size()
	{
		UniqueLock lock(m_mutex);
		return m_queue.size();
	}
	void Stop()
	{
		UniqueLock lock(m_mutex);
		m_need_stop = true;
		m_not_full.notify_all();
		m_not_empty.notify_all();
	}
private:
	void Add(T&& t)
	{
		UniqueLock lock(m_mutex);
		m_not_full.wait(lock, [this]() {return m_need_stop || !IsFull(); });
		if (m_need_stop)
		{
			return;
		}
		m_queue.emplace_back(std::forward<T>(t));
		m_not_empty.notify_one();
	}
private:
	std::list<T> m_queue;
	bool m_need_stop;
	Mutex m_mutex;
	Cond m_not_empty;
	Cond m_not_full;
	int32_t m_max_size;
};
