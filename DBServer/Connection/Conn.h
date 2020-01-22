#pragma once

#include "mariacpp/connection.hpp"
#include "mariacpp/lib.hpp"
#include "mariacpp/resultset.hpp"
#include <memory>

class Conn
{
public:
	Conn();
	~Conn();

	void Init();
	bool ConnectToDB();
	bool IsConnOk();

	void Query(const std::string& sql);

	void DisConnect();
private:
	MariaCpp::Connection m_conn;
	MariaCpp::scoped_library_init maria_lib_init; // ”√¿¥init mysql

	std::shared_ptr<MariaCpp::ResultSet> m_result;
	// prepare model
	std::shared_ptr<MariaCpp::PreparedStatement> m_login_check;
	std::shared_ptr<MariaCpp::PreparedStatement> m_login_insert;
};

