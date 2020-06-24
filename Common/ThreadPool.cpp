#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
	m_thread_n = std::thread::hardware_concurrency();
	Start();
}

ThreadPool::ThreadPool(int32_t n) : m_thread_n(n)
{
	Start();
}

ThreadPool::~ThreadPool()
{
	Stop();
}

void ThreadPool::Start()
{
	m_running = true;
	for (int32_t i=0;i<m_thread_n;i++)
	{
		m_thread_list.emplace_back(std::make_shared<std::thread>(&ThreadPool::Running, this));
	}
}

void ThreadPool::Stop()
{
	std::call_once(m_once, [this] {StopPool(); });
}

void ThreadPool::StopPool()
{
	m_task_queue.Stop();
	m_running = false;
	for (auto& it : m_thread_list)
	{
		it->join();
	}
	m_thread_list.clear();
}

void ThreadPool::AddTask(task_t& task)
{
	m_task_queue.Put(task);
}

void ThreadPool::AddTask(task_t&& task)
{
	m_task_queue.Put(task);
}

void ThreadPool::Running()
{
	while (m_running)
	{
		std::list<task_t> task_list;
		m_task_queue.Take(task_list);
		for (auto& task : task_list)
		{
			if (m_running)
			{
				task();
			}
		}
	}
}
