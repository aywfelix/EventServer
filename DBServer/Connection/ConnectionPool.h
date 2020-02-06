#pragma once
#include <map>
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
	using sqlquery_t = std::map<std::string, std::string>;
	using sqlresult_t = std::map<std::string, MariaCpp::ResultSet*>;
public:

	bool Init();
	void ThreadLoop();
	void AddSqlReq(const std::string& playerid, const std::string& sql);
	bool IsFree() { return m_sqlquery.empty(); }
	int GetReqSize() { return m_sqlquery.size(); }
	void GetQueryRes(const std::string& playerid, result_t& result);
private:
	bool Query(const std::string& playerid, const std::string& sql);
private:
	Conn m_conn;
	sqlquery_t m_sqlquery;
	sqlresult_t m_sqlresult;
};

extern std::unique_ptr<ConnectionPool> g_conn_pool;
