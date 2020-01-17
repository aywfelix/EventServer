#pragma once
#include <deque>
#include <vector>
#include <string>
#include <memory>
#include "ThreadBase.h"
#include "Conn.h"

class ConnThread;

// 通过线程池处理sql请求
// 玩家的数据在上线后保存到内存中，每个玩家每隔5分钟存档一次即可，玩家下线将所有数据保存到数据库
class ConnectionPool
{
public:
	ConnectionPool();
	ConnectionPool(int thrdnum);
	~ConnectionPool();
	void Init();
	ConnThread* Malloc();  // 获取线程对象
	void Free(ConnThread* conn); // 回收一个线程对象
	void Stop();
private:
	int m_conn_num;   //conn thread num
	std::vector<ConnThread*> m_conn_threads;
};

class ConnThread : public ThreadBase
{
public:

	bool Init();
	void ThreadLoop();
	bool IsFree();
	void AddSqlReq(const std::string& sql);
	bool Execute(const std::string& sql);

	int GetReqSize() { return m_sqldeque.size(); }

private:
	Conn m_conn;
	std::deque<std::string> m_sqldeque;
};

extern std::unique_ptr<ConnectionPool> g_conn_pool;
