#pragma once

#include "mariacpp/connection.hpp"
#include "mariacpp/lib.hpp"

class MyConnection
{
public:
	MyConnection();
	~MyConnection();

	bool ConnectToDB();
	bool IsConnOk();

	bool ExecuteSql(const std::string& sql);
	void DisConnect();
private:
	MariaCpp::Connection m_Conn;
	MariaCpp::scoped_library_init maria_lib_init;
};

