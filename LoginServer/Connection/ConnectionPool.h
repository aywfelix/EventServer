#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "ThreadBase.h"
#include "Conn.h"
#include <deque>

class ConnThread;
class LoginPlayer;
class Packet;
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
	ConnThread* GetConnThread(std::thread::id& tid);
private:
	int m_conn_num;   //conn thread num
	std::vector<ConnThread*> m_conn_threads;
};

///////////////////////////////// MsgCache /////////////////

class MsgCache
{
public:
	LoginPlayer* m_player;
	Packet* m_packet;
	std::string m_sql;
	result_t m_result;
};

//////////////////////////// ConnThread ///////////////////////

class ConnThread : public ThreadBase
{
	using reqquery_t = std::map<uint64_t, std::string>;
	using reqresult_t = std::map<uint64_t, MariaCpp::ResultSet*>;
	using reqmsgcache_t = std::deque<MsgCache>;
public:
	bool Init();
	void ThreadLoop();
	void AddReqMsg(MsgCache& msgcache);
	MsgCache* GetMsgCache() { return &m_reqmsgcache.front(); }
	bool IsFree() { return m_reqmsgcache.empty(); }
	int GetReqSize() { return m_reqmsgcache.size(); }
private:
	bool Query(MsgCache& msgcache);
private:
	Conn m_conn;
	reqmsgcache_t m_reqmsgcache;
};

extern std::unique_ptr<ConnectionPool> g_conn_pool;
