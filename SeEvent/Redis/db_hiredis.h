#pragma once

#include "hiredis.h"
#include "SePlatForm.h"
#include <string>
#include <memory>
#include <deque>
#include <mutex>




///////////////////////////////////////////////////////////////////////////////
//using hiredis_t = std::shared_ptr<hiredis>;
class hiredis_pool
{
public:
	enum errors {
		connect_failed = 1,
	};

	hiredis_pool(int n, const char* ip, unsigned int port);
	~hiredis_pool();

	bool clear();

	redisContext* malloc();
	void free(redisContext* redis);
	bool check_connect(redisContext* redis);
private:
	std::deque<redisContext*> m_hiredis_pool;
	uint64_t m_lastcheck_tms;

	std::mutex m_mutex;

	const char* m_ip;
	unsigned int m_port;

	struct timeval m_timeout;
};