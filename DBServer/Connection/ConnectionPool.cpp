#include "ConnectionPool.h"
#include "Util.h"
#include "LogUtil.h"
#include "SePlatForm.h"

std::unique_ptr<ConnectionPool> g_conn_pool = nullptr;

ConnectionPool::ConnectionPool(int thrdnum) :m_conn_num(thrdnum)
{
	m_conn_threads.resize(m_conn_num);
}

ConnectionPool::ConnectionPool()
{
	m_conn_num = GetCpuCores();
	//m_conn_num = 1;
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
		ConnThread* conn_thread = new ConnThread;
		conn_thread->Start();
		m_conn_threads[i] = conn_thread;
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
	m_sqlquery.clear();
	return true;
}

void ConnThread::ThreadLoop()  // 当线程池初始化时，调用此方法
{
	bool bConn = false;
	while (IsActive())
	{
		if (!bConn)
		{
			m_conn.ConnectToDB();
		}
		bConn = m_conn.IsConnOk();
		if (bConn && !m_sqlquery.empty())
		{
			for (auto& it : m_sqlquery)
			{
				Query(it.first, it.second);
			}
		}
		sf_sleep(20);
	}
	//线程结束断开连接
	m_conn.DisConnect();
}

void ConnThread::AddSqlReq(uint64_t playerid, const std::string& sql)
{
	m_sqlquery.emplace(playerid, sql);
}

bool ConnThread::Query(uint64_t playerid, const std::string& sql)
{
	//result_t result = std::unique_ptr<MariaCpp::ResultSet>(m_conn.Query(sql));
	MariaCpp::ResultSet* result = m_conn.Query(sql);
	if (result == nullptr) return false;
	m_sqlresult.emplace(playerid, result);

	// print query result
	//if (result)
	//{
	//	while (result && result->next()) {
	//		std::cout << "playerid = " << result->getInt64(0)
	//			<< ", gameid = '" << result->getRaw(1) << "'"
	//			<< std::endl;
	//	}
	//}
	return true;
}

void ConnThread::GetQueryRes(uint64_t playerid, result_t& result)
{
	auto it = m_sqlresult.find(playerid);
	if (it == m_sqlresult.end()) return;
	result = std::unique_ptr<MariaCpp::ResultSet>(it->second);
}

