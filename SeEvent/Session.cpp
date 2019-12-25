#include "Session.h"
#include "Socket.h"

std::unique_ptr<SessionPool> g_pSessionPool = nullptr;
bool Session::Init()
{
	return true;
}
bool Session::Clear()
{
	if (m_pSocket)
	{
		m_pSocket->CloseSocket();
		delete m_pSocket;
	}
	return true;
}

SessionPool::SessionPool()
{}

SessionPool::~SessionPool()
{}

Session* SessionPool::NewSession()
{
	return m_session_pool.NewElem();
}

MemPool<Session>& SessionPool::AllSession()
{
	return m_session_pool;
}

bool SessionPool::DelSession(Session* session)
{
	return m_session_pool.DelElem(session->GetMemId());
}