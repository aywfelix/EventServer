#include "db_hiredis.h"
#include "LogUtil.h"

//redis_pool
hiredis_pool::hiredis_pool(int n, const char* ip, unsigned int port)
{
	m_ip = ip;
	m_port = port;

	m_timeout = { 1, 500000 };  //1.5 seconds
	redisContext* redis_ctx = nullptr;
	for (int i = 0; i < n; ++i)
	{
		redis_ctx = redisConnectWithTimeout(m_ip, m_port, m_timeout);
		if (redis_ctx && redis_ctx->err == 0)
		{
			m_hiredis_pool.emplace_back(redis_ctx);
			redisEnableKeepAlive(redis_ctx);
		}
		else if(redis_ctx)
		{
			LOG_DEBUG("hiredis: connect redis error, %d", redis_ctx->err);
			break;
		}
	}
}

hiredis_pool::~hiredis_pool()
{
	clear();
}

bool hiredis_pool::clear()
{
	for (auto& it : m_hiredis_pool)
	{
		if (it != nullptr)
		{
			redisFree(it);
			it = nullptr;
		}
	}
	m_hiredis_pool.clear();
	return true;
}

redisContext* hiredis_pool::malloc()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	redisContext* redis_ctx = nullptr;
	if (m_hiredis_pool.empty())
	{
		redis_ctx = redisConnectWithTimeout(m_ip, m_port, m_timeout);
		if (redis_ctx && redis_ctx->err !=0)
		{
			LOG_ERR("connect redis error, %d", redis_ctx->err);
			return nullptr;
		}
		return redis_ctx;
	}
	redis_ctx = m_hiredis_pool.front();
	m_hiredis_pool.pop_front();
	if (!check_connect(redis_ctx))
	{
		if (redisReconnect(redis_ctx) == REDIS_OK)
		{
			redisEnableKeepAlive(redis_ctx);
			return redis_ctx;
		}
		LOG_ERR("connect redis error, %d", redis_ctx->err);
	}
	return redis_ctx;
}

void hiredis_pool::free(redisContext* redis)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (redis)
	{
		m_hiredis_pool.push_back(redis);
	}
}

bool hiredis_pool::check_connect(redisContext* redis)
{
	redisReply* preply = reinterpret_cast<redisReply*>(redisCommand(redis, "PING"));
	if (preply == nullptr)
	{
		return false;
	}
	AutoFree<redisReply> free_reply(preply);
	if (preply->type == REDIS_REPLY_ERROR)
	{
		return false;
	}
	std::string preply_str = std::string(preply->str);
	if (preply_str != std::string("PONG"))
	{
		return false;
	}
	return true;
}

