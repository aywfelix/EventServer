#pragma once

#include "db_hiredis.h"
#include <memory>

// 用于具体业务的redis 接口
class db_redis
{
public:
	db_redis();
	~db_redis();
	void init(int n, const char* ip, unsigned int port);
	// key op
	void set_key_val(const char* key, const char* val);
	const char* get_val_by_key(const char* key);
	void del_key(const char* key);
	bool is_exist_key(const char* key);
	// list op
	void lpush_key_val(const char* key, const char* val);
	const char* rpop_key(const char* key);
	// set op
	//void sadd_key_val(const char* key, const char* val);
	//void smembers_key(const char* key);
	//void spop_key(const char* key);

	hiredis_pool* get_pool()
	{
		return m_redis_pool;
	}
private:
	hiredis_pool* m_redis_pool{ nullptr };
};

extern std::unique_ptr<db_redis> g_pRedis;