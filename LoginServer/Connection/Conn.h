#pragma once

#include "mariacpp/connection.hpp"
#include "mariacpp/lib.hpp"

class Conn
{
public:
	Conn();
	~Conn();

	bool ConnectToDB();
	bool IsConnOk();

	bool ExecuteSql(const std::string& sql);
	void DisConnect();
private:
	MariaCpp::Connection m_conn;
	MariaCpp::scoped_library_init maria_lib_init;
};

// loginserver 需要单独连接数据处理登录数据验证