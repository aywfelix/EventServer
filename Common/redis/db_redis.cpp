#include "db_redis.h"
#include "Util.h"
#include "hiredis.h"

std::unique_ptr<db_redis> g_pRedis = nullptr;

db_redis::db_redis() {}
db_redis::~db_redis()
{
	if (m_redis_pool != nullptr)
		delete m_redis_pool;
}

void db_redis::init(int n, const char* ip, unsigned int port)
{
	m_redis_pool = new hiredis_pool(n, ip, port);
}

void db_redis::set_key_val(const char* key, const char* val)
{
	auto redis_ctx = m_redis_pool->malloc();
	if (redis_ctx == nullptr)
	{
		return;
	}
	redisReply* reply = reinterpret_cast<redisReply*>(redisCommand(redis_ctx, "SET %s %s", key, val));
	AutoFree<redisReply> free_reply(reply);
	m_redis_pool->free(redis_ctx);
	if (reply == nullptr || reply->type == REDIS_REPLY_ERROR)
	{
		return;
	}
}

const char* db_redis::get_val_by_key(const char* key)
{
	auto redis_ctx = m_redis_pool->malloc();
	if (redis_ctx == nullptr)
	{
		return nullptr;
	}
	redisReply* reply = reinterpret_cast<redisReply*>(redisCommand(redis_ctx, "GET %s", key));
	AutoFree<redisReply> free_reply(reply);
	m_redis_pool->free(redis_ctx);
	if (reply == nullptr || reply->type == REDIS_REPLY_ERROR)
	{
		return nullptr;
	}
	return reply->str;
}

void db_redis::del_key(const char* key)
{
	auto redis_ctx = m_redis_pool->malloc();
	if (redis_ctx == nullptr)
	{
		return;
	}
	redisReply* reply = reinterpret_cast<redisReply*>(redisCommand(redis_ctx, "DEL %s", key));
	AutoFree<redisReply> free_reply(reply);
	m_redis_pool->free(redis_ctx);
}

bool db_redis::is_exist_key(const char* key)
{
	auto redis_ctx = m_redis_pool->malloc();
	if (redis_ctx == nullptr)
	{
		return false;
	}
	redisReply* reply = reinterpret_cast<redisReply*>(redisCommand(redis_ctx, "EXISTS %s", key));
	AutoFree<redisReply> free_reply(reply);
	m_redis_pool->free(redis_ctx);
	if (reply == nullptr || reply->type == REDIS_REPLY_ERROR)
	{
		return false;
	}
	int ret = (int)reply->integer;
	return ret == 1;
}

void db_redis::lpush_key_val(const char* key, const char* val)
{
	auto redis_ctx = m_redis_pool->malloc();
	if (redis_ctx == nullptr)
	{
		return;
	}
	redisReply* reply = reinterpret_cast<redisReply*>(redisCommand(redis_ctx, "LPUSH %s %s", key, val));
	AutoFree<redisReply> free_reply(reply);
	m_redis_pool->free(redis_ctx);
}

const char* db_redis::rpop_key(const char* key)
{
	auto redis_ctx = m_redis_pool->malloc();
	if (redis_ctx == nullptr)
	{
		return nullptr;
	}
	redisReply* reply = reinterpret_cast<redisReply*>(redisCommand(redis_ctx, "RPOP %s", key));
	AutoFree<redisReply> free_reply(reply);
	m_redis_pool->free(redis_ctx);
	if (reply == nullptr || reply->type == REDIS_REPLY_ERROR)
	{
		return nullptr;
	}
	return reply->str;
}