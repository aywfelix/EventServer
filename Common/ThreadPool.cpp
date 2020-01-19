#include "ThreadPool.h"

void ThreadPool::Start(int32_t nNumThreads)
{
	if(nNumThreads == 0 && threadInitCb)
	{
		threadInitCb();
		return;
	}

	for(int i=0;i<nNumThreads;++i)
	{
		auto thrd = new std::thread(std::bind(&ThreadPool::ThreadFunc, this));
		m_vThreads.push_back(thrd);
	}
	m_bRunning = true;
}

void ThreadPool::Run(const Task& f)
{
	if(m_vThreads.empty())
	{
		return f();
	}
	UniqueLock Lock(m_Mutex);
	while(IsFull())
	{
		m_NotFullCond.wait(Lock);
	}
	m_dTaskQueue.push_back(f);
	m_NotEmptyCond.notify_all();

}

void ThreadPool::Run(Task&& f)
{
	if(m_vThreads.empty())
	{
		return f();
	}
	UniqueLock Lock(m_Mutex);
	while(IsFull())
	{
		m_NotFullCond.wait(Lock);
	}
	m_dTaskQueue.push_back(std::move(f));
	m_NotEmptyCond.notify_all();
}
#include <iostream>
void ThreadPool::Stop()
{
	UniqueLock Lock(m_Mutex);
	m_bRunning = false;
	m_NotEmptyCond.notify_all();
	for(auto it = m_vThreads.begin();it != m_vThreads.end();)
	{
		it = m_vThreads.erase(it);
	}
	std::cout <<"stop thread pool" << std::endl;
}

size_t ThreadPool::GetTaskQueueSize()
{
	UniqueLock Lock(m_Mutex);
	return m_dTaskQueue.size();
}

void ThreadPool::ThreadFunc()
{
	if(threadInitCb)
	{
		threadInitCb();
	}
	while(m_bRunning)
	{
		Task task = TaskTake();
		if(task)
		{
			task();
		}
	}
}

ThreadPool::Task ThreadPool::TaskTake()
{
	UniqueLock Lock(m_Mutex);
	while(m_dTaskQueue.empty() && m_bRunning)
	{
		m_NotEmptyCond.wait(Lock);
	}
	Task task;
	if(!m_dTaskQueue.empty())
	{
		task = m_dTaskQueue.front();
		m_dTaskQueue.pop_front();
		m_NotFullCond.notify_all();
	}
	return task;
}