#pragma once

#include "Lock.hpp"
#include <functional>
#include <vector>
#include <deque>
#include <thread>


class ThreadPool
{
	typedef std::vector<std::thread*> ThreadPoolType;
	typedef std::function<void()> Task;
	typedef std::deque<Task> TaskQueueType;
public:
	 static ThreadPool* Instance()
	 {
	 	static ThreadPool pool;
	 	return &pool;
	 }

	void SetQueueMaxSize(size_t nMaxSize)
	{
		m_nMaxQueueSize = nMaxSize;
	}
	void Start(int32_t nNumThreads);
	void Run(const Task& f);
	void Run(Task&& f);
	void Stop();
	size_t GetTaskQueueSize();
	void SetThreadInitCb(const Task& cb)
	{
		threadInitCb = cb;
	}
private:
	void ThreadFunc();
	Task TaskTake();
	bool IsFull()
	{
		return m_nMaxQueueSize>0 && m_dTaskQueue.size() >= m_nMaxQueueSize;
	}
private:
	int32_t m_nThreadNum{0};
	size_t m_nMaxQueueSize{0};

	ThreadPoolType m_vThreads;
	TaskQueueType m_dTaskQueue;
	Mutex m_Mutex;
	Condition m_NotEmptyCond;
	Condition m_NotFullCond;

	Task threadInitCb;
	bool m_bRunning{false};
};