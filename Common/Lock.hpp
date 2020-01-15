#pragma once

#include <mutex>
#include <condition_variable>
#include <atomic>


typedef std::recursive_mutex RecursiveMutex;
typedef std::mutex Mutex;
typedef std::lock_guard<std::mutex> GuardLock;
typedef std::unique_lock<std::mutex> UniqueLock;

typedef std::condition_variable Condition;

class AtomicLock
{
public:
	AtomicLock()
	{
		flag.clear();
	}
	~AtomicLock()
	{
	}
	void Lock()
	{
		while (flag.test_and_set(std::memory_order_acquire));
	}
	bool TryLock()
	{
		if (flag.test_and_set(std::memory_order_acquire))
		{
			return false;
		}
		return true;
	}
	void UnLock()
	{
		flag.clear(std::memory_order_acquire);
	}
private:
	mutable std::atomic_flag flag = ATOMIC_FLAG_INIT;
	AtomicLock(const AtomicLock&) = delete;
	AtomicLock& operator = (const AtomicLock&) = delete;
};

