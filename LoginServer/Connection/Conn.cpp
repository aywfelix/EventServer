#include "Conn.h"
#include "mariacpp/exception.hpp"
#include "mariacpp/prepared_stmt.hpp"
#include "mariacpp/time.hpp"
#include "mariacpp/uri.hpp"
#include "JsonConfig.h"
#include "LogUtil.h"

Conn::Conn()
{
	//m_login_check = nullptr;
	//m_login_insert = nullptr;
}

Conn::~Conn()
{
}

void Conn::Init()
{
	// 保留常用的查询sql 即安全也提高性能
	//m_login_check = std::make_shared<MariaCpp::PreparedStatement>(m_conn, nullptr);
	//m_login_check->prepare("select * from tb_account where loginname=? and loginpwd=?");
}

bool Conn::ConnectToDB()
{
	bool bConn = false;
	try
	{
	const char* uri = g_pConfig->m_dbConf["uri"].asCString();
	const char* user = g_pConfig->m_dbConf["user"].asCString();
	const char* passwd = g_pConfig->m_dbConf["passwd"].asCString();
	bConn = m_conn.connect(MariaCpp::Uri(uri), user, passwd);
	}
	catch (const std::exception & e)
	{
		LOG_ERR("connect mariadb error,%s", e.what());
		return false;
	}
	return bConn;
}

bool Conn::IsConnOk()
{
	if (m_conn.MyPing() != 0)
	{
		return false;
	}
	return true;
}

void Conn::DisConnect()
{
	m_conn.close();
}

MariaCpp::ResultSet* Conn::Query(const std::string& sql)
{
	m_conn.query(sql);
	return m_conn.store_result();
}