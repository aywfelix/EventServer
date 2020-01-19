/****************************************************************************
  Copyright (C) 2015 Karol Roslaniec <mariacpp@roslaniec.net>
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not see <http://www.gnu.org/licenses>
  or write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*****************************************************************************/
#include <mariacpp/connection.hpp>
#include <mariacpp/exception.hpp>
#include <mariacpp/prepared_stmt.hpp>
#include <mariacpp/resultset.hpp>
#include <mariacpp/uri.hpp>
#include <cassert>
#include <memory>
#include <sstream>

#if __cplusplus < 201103L
# define unique_ptr auto_ptr
#endif

namespace MariaCpp {


Connection::Connection()
{
    mysql_init(&mysql);
}


Connection::~Connection()
{
    close();
}


bool
Connection::connect(const Uri &uri, const char *usr, const char *passwd,
                    unsigned long clientflag)
{
    // unsigned int protocol = 0;
    // switch(uri.protocol()) {
    // case Uri::PROTOCOL_TCP: protocol = MYSQL_PROTOCOL_TCP; break;
    // case Uri::PROTOCOL_SOCKET: protocol = MYSQL_PROTOCOL_SOCKET; break;
    // case Uri::PROTOCOL_PIPE: protocol = MYSQL_PROTOCOL_PIPE; break;
    // default: break;
    // }
    // if (protocol) options(MYSQL_OPT_PROTOCOL, &protocol);
    
    return connect(uri.host(), usr, passwd, uri.schema(), uri.port(),
                   uri.socket(), clientflag);
}


void
Connection::close()
{
    // Calling mysql_close() twice would crash MariaDB Connector/C,
    // therefore we have to protect against it
    if (mysql.methods) mysql_close(&mysql);
    mysql.methods = NULL;
}


bool
Connection::connect(const char *host,
                    const char *user,
                    const char *passwd,
                    const char *db,
                    unsigned int port,
                    const char *unix_socket,
                    unsigned long clientflag)
{
	if (!mysql_real_connect(&mysql, host, user, passwd, db, port,
		unix_socket, clientflag))
	{
		throw_exception();
		return false;
	}
        
	return true;
}


std::string
Connection::escape_string(const std::string &s)
{
    if (s.empty()) return s;
    std::string buffer(s.size() * 2 + 1, '\0');
    unsigned long len = escape_string(&buffer[0], s.c_str(), s.size());
    buffer.resize(len);
    return buffer;
}


ResultSet *
Connection::list_dbs(const char *wild)
{
    MYSQL_RES *res = mysql_list_dbs(&mysql, wild);
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : 0;
}


ResultSet *
Connection::list_fields(const char *table, const char *wild)
{
    MYSQL_RES *res = mysql_list_fields(&mysql, table, wild);
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : 0;
}


ResultSet *
Connection::list_processes()
{
    MYSQL_RES *res = mysql_list_processes(&mysql);
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : 0;
}


ResultSet *
Connection::list_tables(const char *wild)
{
    MYSQL_RES *res = mysql_list_tables(&mysql, wild);
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : 0;
}


bool
Connection::next_result()
{
    CC();
    int res = mysql_next_result(&mysql);
    if (0 < res) throw_exception();
    return !res;
}


void
Connection::options(enum mysql_option option, const void *arg)
{
    if (!mysql_options(&mysql, option, arg)) return;
    std::ostringstream msg;
    msg << "Invalid options argument =" << option;
    throw InvalidArgumentException(msg.str());
}


PreparedStatement *
Connection::prepare(const std::string &sql)
{
    std::unique_ptr<PreparedStatement> res(stmt_init());
    res->prepare(sql);
    return res.release();
}


void
Connection::set_local_infile_handler(
    int (*local_infile_init)(void **, const char *, void *),
    int (*local_infile_read)(void *, char *, unsigned int),
    void (*local_infile_end)(void *),
    int (*local_infile_error)(void *, char*, unsigned int),
    void *userdata)
{
    return mysql_set_local_infile_handler(&mysql,
                                          local_infile_init,
                                          local_infile_read,
                                          local_infile_end,
                                          local_infile_error,
                                          userdata);
}


const char *
Connection::stat()
{
    const char *res = mysql_stat(&mysql);
    if (!res) throw_exception();
    return res;
}


PreparedStatement *
Connection::stmt_init()
{
    MYSQL_STMT *stmt = mysql_stmt_init(&mysql);
    if (!stmt) throw_exception();
    return new PreparedStatement(*this, stmt);
}


ResultSet *
Connection::store_result()
{
    MYSQL_RES *res = mysql_store_result(&mysql);
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : 0;
}


ResultSet *
Connection::use_result()
{
    MYSQL_RES *res = mysql_use_result(&mysql);
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : 0;
}


void
Connection::throw_exception()
{
    throw Exception(error_str(), errorno(), sqlstate());
}


}
