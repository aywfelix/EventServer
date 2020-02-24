#include "ConnectionPool.h"
#include "Util.h"
#include "LogUtil.h"
#include "SePlatForm.h"
#include "Common/packet/Packet.h"
#include "clientmodule/ModuleLogin.h"

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

ConnThread* ConnectionPool::GetConnThread(std::thread::id& tid)
{
	for (int i = 0; i < m_conn_num; i++)
	{
		if (m_conn_threads[i]->GetThreadId() == tid)
		{
			return m_conn_threads[i];
		}
	}
	return nullptr;
}

///////////////////////////////// ConnThread  ///////////////////////////////////////

bool ConnThread::Init()
{
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
		if (bConn && !m_reqmsgcache.empty())
		{
			MsgCache& msg = m_reqmsgcache.front();
			Query(msg);
			//auto pHandle = g_packetmgr->GetMsgHandle(msg.m_packet->msg_id);
			//pHandle(msg.m_player, msg.m_packet);
			m_reqmsgcache.pop_front();
		}
		sf_sleep(20);
	}
	//线程结束断开连接
	m_conn.DisConnect();
}

void ConnThread::AddReqMsg(MsgCache& msgcache)
{
	m_reqmsgcache.push_back(msgcache);
}

bool ConnThread::Query(MsgCache& msgcache)
{
	MariaCpp::ResultSet* result = m_conn.Query(msgcache.m_sql);
	if (result == nullptr) return false;
	msgcache.m_result = std::unique_ptr<MariaCpp::ResultSet>(result);
	return true;
}
