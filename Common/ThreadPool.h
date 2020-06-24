#include <list>
#include <functional>
#include <atomic>
#include <thread>
#include "SyncQueue.hpp"
#include "SingleTon.hpp"

class ThreadPool : public SingleTon<ThreadPool>
{
	using task_t = std::function<void()>;
	using thread_list_t = std::list<std::shared_ptr<std::thread>>;
public:
	ThreadPool();
	ThreadPool(int32_t n);
	~ThreadPool();

	void Start();
	void Stop();
	void AddTask(task_t& task);
	void AddTask(task_t&& task);
private:
	void Running();
	void StopPool();
private:
	int32_t m_thread_n;
	thread_list_t m_thread_list;
	SyncQueue<task_t> m_task_queue;
	std::atomic_bool m_running;
	std::once_flag m_once;
};