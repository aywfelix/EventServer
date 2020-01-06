#pragma once

#include "ThreadBase.h"
#include "Connection/MyConnection.h"
#include <deque>
#include <string>
#include <memory>

class ConnThread;

class ConnectionPool
{
public:
	ConnectionPool();
	ConnectionPool(int thrdnum);
	~ConnectionPool();
	void Init();
	void DispatchReq(const std::string& sql);
	void Stop();
private:
	int m_ConnNum;   //conn thread num
	ConnThread** m_ConnThrds;
};

class ConnThread : public ThreadBase
{
public:

	bool Init();
	void ThreadLoop();
	bool IsFree();
	void AddSqlReq(const std::string& sql);
	bool Execute(const std::string& sql);

	int GetReqSize() { return m_SqlDeque.size(); }

private:
	MyConnection m_Conn;
	std::deque<std::string> m_SqlDeque;
};

extern std::unique_ptr<ConnectionPool> g_pConnPool;
