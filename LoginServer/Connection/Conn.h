#pragma once

#include "mariacpp/connection.hpp"
#include "mariacpp/lib.hpp"
#include "mariacpp/resultset.hpp"
#include "mariacpp/prepared_stmt.hpp"
#include <memory>

using result_t = std::shared_ptr<MariaCpp::ResultSet>;
using prepared_stmt_t = std::unique_ptr<MariaCpp::PreparedStatement>;
class Conn
{
public:
	Conn();
	~Conn();

	void Init();
	bool ConnectToDB();
	bool IsConnOk();

	MariaCpp::ResultSet* Query(const std::string& sql);

	void DisConnect();
private:
	MariaCpp::Connection m_conn;
	MariaCpp::scoped_library_init maria_lib_init; // 用来init mysql

	// prepare model
	//MariaCpp::PreparedStatement m_login_check;
	//MariaCpp::PreparedStatement m_login_insert;
};

