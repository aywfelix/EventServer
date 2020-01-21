#include "SFTest.h"
#include "LogUtil.h"
#include "redis/db_redis.h"

void SFTest::TestRedis()
{
	db_redis redis;
	redis.init(5, "192.168.40.182", 6379);
	const char* key = "playerid";
	redis.set_key_val(key, "123456789");
	LOG_INFO("get key is %s", redis.get_val_by_key(key));
	if (redis.is_exist_key(key))
	{
		redis.del_key(key);
		LOG_INFO("del key ok %s", key);
	}

	const char* key2 = "playerids";
	redis.lpush_key_val(key2, "1111111111");
	redis.lpush_key_val(key2, "2222222222");
	redis.lpush_key_val(key2, "3333333333");
	LOG_INFO("rpop value is %s", redis.rpop_key(key2));
	LOG_INFO("rpop value is %s", redis.rpop_key(key2));
	LOG_INFO("rpop value is %s", redis.rpop_key(key2));
}