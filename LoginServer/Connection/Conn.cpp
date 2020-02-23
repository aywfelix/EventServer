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
	const char* uri = g_JsonConfig->m_dbConf["uri"].asCString();
	const char* user = g_JsonConfig->m_dbConf["user"].asCString();
	const char* passwd = g_JsonConfig->m_dbConf["passwd"].asCString();
	bConn = m_conn.connect(MariaCpp::Uri(uri), user, passwd);
	}
	catch (const std::exception & e)
	{
		CLOG_ERR << "connect mariadb error," << e.what() << CLOG_END;
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
	// next() is an alias for fetch_row()
	//MariaCpp::ResultSet* res_set = m_result.get();
	//while (res_set && res_set->next()) {
	//	std::cout << "id = " << res_set->getInt(0)
	//		<< ", label = '" << res_set->getRaw(1) << "'"
	//		<< ", date = "
	//		<< (res_set->isNull(2) ? "NULL" : res_set->getString(2).c_str())
	//		<< std::endl;
	//}
}