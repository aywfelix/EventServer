#include "SFTest.h"
#include "Connection/ConnectionPool.h"
#include "Connection/Conn.h"
#include <memory>
#include "JsonConfig.h"

void SFTest::TestConnMysql()
{
	//server config
	g_JsonConfig.reset(new JsonConfig());
	g_JsonConfig->Load("../Config/ServerConf.json");
	g_JsonConfig->m_ServerConf = g_JsonConfig->m_Root["DBServer"];
	//mariadb config
	g_JsonConfig->m_dbConf = g_JsonConfig->m_Root["MariaDB"];

	//Conn conn;
	//bool bconn = conn.ConnectToDB();
	g_conn_pool = std::make_unique<ConnectionPool>();
	g_conn_pool->Init();
	ConnThread* thrd = g_conn_pool->Malloc();
	std::string sql = "select * from tb_player";
	thrd->AddSqlReq(1, sql);

	sf_sleep(1000);
	result_t result;
	thrd->GetQueryRes(1, result);

}