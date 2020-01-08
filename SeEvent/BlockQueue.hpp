#pragma once

#include <deque>
#include "Lock.hpp"

template<typename T>
class Queue {
public:
	Queue()
	{
	}
	virtual ~Queue()
	{
		m_cond_empty.notify_all();
		if (!m_deque.empty())
		{
			m_deque.clear();
		}
	}

	void PutQ(const T& info)
	{
		UniqueLock lock(m_mutex);
		m_deque.push_back(info);
		m_cond_empty.notify_one();
	}

	bool PopQ(T& info)
	{
		UniqueLock lock(m_mutex);
		if (m_deque.empty())
		{
			while (m_cond_empty.wait_for(lock, std::chrono::milliseconds(100)) == std::cv_status::timeout)
			{
				return false;
			}
			if (m_deque.empty())
				return false;
		}
		info = m_deque.front();
		m_deque.pop_front();
		return true;
	}

private:
	std::deque<T> m_deque;
	Mutex m_mutex;
	Condition m_cond_empty;
};
