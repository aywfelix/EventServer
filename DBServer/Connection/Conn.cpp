#include "Conn.h"
#include "mariacpp/exception.hpp"
#include "mariacpp/prepared_stmt.hpp"
#include "mariacpp/time.hpp"
#include "mariacpp/uri.hpp"
#include "JsonConfig.h"
#include "LogUtil.h"

Conn::Conn()
{
}

Conn::~Conn()
{
}


bool Conn::ConnectToDB()
{
	const char* uri = g_JsonConfig->m_dbConf["uri"].asCString();
	const char* user = g_JsonConfig->m_dbConf["root"].asCString();
	const char* passwd = g_JsonConfig->m_dbConf["passwd"].asCString();
	try
	{
		m_conn.connect(MariaCpp::Uri(uri), user, passwd);
	}
	catch (const std::exception & e)
	{
		CLOG_ERR << "connect mariadb error," << e.what() << CLOG_END;
		return false;
	}
	return true;
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

bool Conn::ExecuteSql(const std::string& sql)
{
	m_conn.query(sql);
	return true;
}