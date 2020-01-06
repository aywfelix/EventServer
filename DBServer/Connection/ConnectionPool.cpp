#include "ConnectionPool.h"
#include "Util.h"

ConnectionPool::ConnectionPool(int thrdnum):m_ConnNum(thrdnum)
{
	Init();
}

ConnectionPool::ConnectionPool()
{
	m_ConnNum = 8;
	Init();
}

ConnectionPool::~ConnectionPool()
{
}

void ConnectionPool::Stop()
{
	for (int i = 0; i < m_ConnNum; ++i)
	{
		m_ConnThrds[i]->Stop();
		DELETE_PTR(m_ConnThrds[i]);
	}
	DELETE_PTR_ARR(m_ConnThrds);
}

void ConnectionPool::Init()
{
	m_ConnThrds = new ConnThread*[m_ConnNum];
	for (int i = 0; i < m_ConnNum; ++i)
	{
		ConnThread* thrd = new ConnThread;
		thrd->Init();
		thrd->Start();
		m_ConnThrds[i] = thrd;
	}
}

void ConnectionPool::DispatchReq(const std::string& sql)
{
	//1、根据负载情况进行分发请求 //2、直接通过hash 或者随机方式
	for (int i=0;i<m_ConnNum;i++)
	{
		if (m_ConnThrds[i]->IsFree())
		{
			m_ConnThrds[i]->AddSqlReq(sql);
			return;
		}
	}
	int nSize = 0;
	int idx = 0;
	for (int i = 0; i < m_ConnNum; i++)
	{
		int nReqSize = m_ConnThrds[i]->GetReqSize();
		if (nSize == 0)
		{
			nSize = nReqSize;
		}
		if (nSize >= nReqSize)
		{
			nSize = nReqSize;
			idx = i;
		}
	}

	m_ConnThrds[idx]->AddSqlReq(sql);
}


bool ConnThread::Init()
{
	m_SqlDeque.clear();

	return true;
}

void ConnThread::ThreadLoop()
{
	bool bConn = false;
	while (IsActive())
	{
		if (!bConn)
		{
			m_Conn.ConnectToDB();
		}
		bConn = m_Conn.IsConnOk();
		if (bConn && !m_SqlDeque.empty())
		{
			std::string sql = m_SqlDeque.front();
			Execute(sql);
		}
		SFSLEEP(20);
	}
	//线程结束断开连接
	m_Conn.DisConnect();
}

bool ConnThread::IsFree()
{
	return true;
}

void ConnThread::AddSqlReq(const std::string& sql)
{
	m_SqlDeque.emplace_back(sql);
}

bool ConnThread::Execute(const std::string& sql)
{
	m_Conn.ExecuteSql(sql);
	return true;
}

std::unique_ptr<ConnectionPool> g_pConnPool = nullptr;