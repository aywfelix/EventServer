#include "ConnectionPool.h"
#include "Util.h"
#include "LogUtil.h"
#include "SePlatForm.h"
#include "Common/packet/Packet.h"
#include "MsgHandle/LoginPlayer.h"
#include "client/login/HandleLogin.h"

std::unique_ptr<ConnectionPool> g_conn_pool = nullptr;

ConnectionPool::ConnectionPool(int thrdnum) :m_conn_num(thrdnum)
{
	m_conn_threads.resize(m_conn_num);
}

ConnectionPool::ConnectionPool()
{
	m_conn_num = GetCpuCores();
	m_conn_num = 1;
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
		thread->Clear();
		thread->Stop();
		DELETE_PTR(thread);
	}
	m_conn_threads.clear();
}

void ConnectionPool::Init()
{
	for (int i = 0; i < m_conn_num; ++i)
	{
		ConnThread* thread = new ConnThread;
		thread->Start();
		m_conn_threads[i] = thread;
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
	m_conn.Init();
	m_reqmsgcache.clear();
	return true;
}


void ConnThread::ThreadLoop()
{
	while (IsActive())
	{
		if (!m_conn.IsConnOk())
		{
			if (m_conn.ConnectToDB())
			{
				LOG_INFO("conn database ok!!!");
			}
		}
		else 
		{
			if (!m_reqmsgcache.empty())
			{
				try
				{
					MsgCache& msg = m_reqmsgcache.front();
					Query(msg);
					auto pHandle = g_pPacketMgr->GetMsgHandle(msg.m_packet->msg_id);
					pHandle(msg.m_player, msg.m_packet);
					m_reqmsgcache.pop_front();
				}
				catch (const std::exception & e)
				{
					LOG_FATAL("LoginServer: query error(%s)", e.what());
				}
			}
		}
		sf_sleep(20);
	}
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
