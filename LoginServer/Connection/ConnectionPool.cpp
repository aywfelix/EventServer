#include "ConnectionPool.h"
#include "Util.h"
#include "LogHelper.h"

ConnectionPool::ConnectionPool(int thrdnum):m_conn_num(thrdnum)
{
	m_conn_threads.resize(m_conn_num);
}

ConnectionPool::ConnectionPool()
{
	m_conn_num = 8;
	m_conn_threads.resize(m_conn_num);
}

ConnectionPool::~ConnectionPool()
{
}

void ConnectionPool::Stop()
{
	for (int i = 0; i < m_conn_num; ++i)
	{
		ConnThread* thread = m_conn_threads[i];
		thread->Stop();
		DELETE_PTR(thread);
		thread = nullptr;
	}
	m_conn_threads.clear();
}

void ConnectionPool::Init()
{
	for (int i = 0; i < m_conn_num; ++i)
	{
		ConnThread* thread = new ConnThread;
		thread->Init();
		thread->Start();
		m_conn_threads.push_back(thread);
	}
}

ConnThread* ConnectionPool::Malloc()
{
	// 1、根据负载情况进行分发请求 
	// 2、直接通过hash 或者随机方式
	for (int i = 0; i < m_conn_num; i++)
	{
		if (m_conn_threads[i]->IsFree())
		{
			return m_conn_threads[i];
		}
	}
	int nSize = 0;
	int idx = 0;
	for (int i = 0; i < m_conn_num; i++)
	{
		int nReqSize = m_conn_threads[i]->GetReqSize();
		if (nSize == 0) nSize = nReqSize;
		if (nSize >= nReqSize)
		{
			nSize = nReqSize;
			idx = i;
		}
	}
	return m_conn_threads[idx];
}

void ConnectionPool::Free(ConnThread* conn)
{
	if (conn == nullptr) return;
	if (m_conn_threads.size() >= m_conn_num)
	{
		LOG_FATAL("some conn free more times");
	}
	m_conn_threads.push_back(conn);
}

bool ConnThread::Init()
{
	m_sqldeque.clear();
	return true;
}

void ConnThread::ThreadLoop()
{
	bool bConn = false;
	while (IsActive())
	{
		if (!bConn)
		{
			m_conn.ConnectToDB();
		}
		bConn = m_conn.IsConnOk();
		if (bConn && !m_sqldeque.empty())
		{
			std::string sql = m_sqldeque.front();
			Execute(sql);
		}
		sf_sleep(20);
	}
	//线程结束断开连接
	m_conn.DisConnect();
}

bool ConnThread::IsFree()
{
	return true;
}

void ConnThread::AddSqlReq(const std::string& sql)
{
	m_sqldeque.emplace_back(sql);
}

bool ConnThread::Execute(const std::string& sql)
{
	m_conn.ExecuteSql(sql);
	return true;
}

std::unique_ptr<ConnectionPool> g_conn_pool = nullptr;