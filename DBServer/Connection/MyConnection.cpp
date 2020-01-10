#include "MyConnection.h"
#include "mariacpp/exception.hpp"
#include "mariacpp/prepared_stmt.hpp"
#include "mariacpp/time.hpp"
#include "mariacpp/uri.hpp"
#include "JsonConfig.h"
#include "LogHelper.h"

MyConnection::MyConnection()
{
}

MyConnection::~MyConnection()
{
}


bool MyConnection::ConnectToDB()
{
	const char* uri = g_JsonConfig->m_dbConf["uri"].asCString();
	const char* user = g_JsonConfig->m_dbConf["root"].asCString();
	const char* passwd = g_JsonConfig->m_dbConf["passwd"].asCString();
	try
	{
		m_Conn.connect(MariaCpp::Uri(uri), user, passwd);
	}
	catch (const std::exception & e)
	{
		CLOG_ERR << "connect mariadb error," << e.what() << CLOG_END;
		return false;
	}
	return true;
}

bool MyConnection::IsConnOk()
{
	if (m_Conn.MyPing() != 0)
	{
		return false;
	}
	return true;
}

void MyConnection::DisConnect()
{
	m_Conn.close();
}

bool MyConnection::ExecuteSql(const std::string& sql)
{
	m_Conn.query(sql);
	return true;
}