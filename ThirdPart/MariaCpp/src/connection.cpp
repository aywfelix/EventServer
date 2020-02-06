/****************************************************************************
  Copyright (C) 2015-2019 Karol Roslaniec <mariacpp@roslaniec.net>
  
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
#include <cstring>
#include <memory>
#include <sstream>
#ifdef MARIADB_VERSION_ID
#include <poll.h>
#endif

#if __cplusplus < 201103L
# define unique_ptr auto_ptr
#endif

namespace MariaCpp {


Connection::Connection()
#   ifdef MARIADB_VERSION_ID
    : _async_status()
#   endif
{
    mysql_init(&mysql);
}


Connection::~Connection()
{
    close();
}


void
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


void
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
        throw_exception();
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


#if 50600 <= MYSQL_VERSION_ID
void
Connection::options4(enum mysql_option option,
                     const void *arg1, const void *arg2)
{
    if (!mysql_options4(&mysql, option, arg1, arg2)) return;
    std::ostringstream msg;
    msg << "Invalid options argument =" << option;
    throw InvalidArgumentException(msg.str());
}
#endif


PreparedStatement *
Connection::prepare(const std::string &sql)
{
    std::unique_ptr<PreparedStatement> res(new PreparedStatement(*this));
    res->prepare(sql);
    return res.release();
}


#if 50700 <= MYSQL_VERSION_ID
std::string
Connection::session_track_get_first(enum enum_session_state_type type)
{
    CC();
    const char *data;
    size_t len;
    if (mysql_session_track_get_first(&mysql, type, &data, &len))
        throw_exception();
    return std::string(data, len);
}


bool
Connection::session_track_get_next(enum enum_session_state_type type,
                                   std::string &str)
{
    CC();
    const char *data;
    size_t len;
    int res = mysql_session_track_get_next(&mysql, type, &data, &len);
    if (!res) str.assign(data, len);
    return !res;
}
#endif


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


MYSQL_STMT *
Connection::stmt_init()
{
    MYSQL_STMT *stmt = mysql_stmt_init(&mysql);
    if (!stmt) throw_exception();
    return stmt;
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



#ifdef MARIADB_VERSION_ID
//
//  MariaDB-specific non-blocking functions:
//

int
Connection::async_wait()
{
    assert(_async_status);
    struct pollfd pfd;
    int res = -1;

    pfd.fd = get_socket();
    pfd.events=
        (_async_status & MYSQL_WAIT_READ ? POLLIN : 0) |
        (_async_status & MYSQL_WAIT_WRITE ? POLLOUT : 0) |
        (_async_status & MYSQL_WAIT_EXCEPT ? POLLPRI : 0);
    int timeout = _async_status & MYSQL_WAIT_TIMEOUT
        ? get_timeout_value_ms() : -1;

    do {
        res= poll(&pfd, 1, timeout);
    } while (res == -1 && errno == EINTR);

    if (res == 0)
        return MYSQL_WAIT_TIMEOUT;
    else if (res < 0)
    {
        /*
          In a real event framework, we should handle EINTR and re-try the poll.
        */
        return MYSQL_WAIT_TIMEOUT;
    }
    else
    {
        int status= 0;
        if (pfd.revents & POLLIN)
            status|= MYSQL_WAIT_READ;
        if (pfd.revents & POLLOUT)
            status|= MYSQL_WAIT_WRITE;
        if (pfd.revents & POLLPRI)
            status|= MYSQL_WAIT_EXCEPT;
        return status;
    }
}


void
Connection::autocommit_start(bool mode)
{
    assert(!_async_status);
    my_bool ret;
    _async_status = mysql_autocommit_start(&ret, &mysql, mode);
    if (!_async_status && ret) throw_exception();
}


void
Connection::autocommit_cont(int status)
{
    assert(_async_status);
    my_bool ret;
    _async_status = mysql_autocommit_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::commit_start()
{
    assert(!_async_status);
    my_bool ret;
    _async_status = mysql_commit_start(&ret, &mysql);
    if (!_async_status && ret) throw_exception();
}


void
Connection::commit_cont(int status)
{
    assert(_async_status);
    my_bool ret;
    _async_status = mysql_commit_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::dump_debug_info_start()
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_dump_debug_info_start(&ret, &mysql);
    if (!_async_status && ret) throw_exception();
}


void
Connection::dump_debug_info_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_dump_debug_info_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::connect_start(const Uri &uri, const char *usr, const char *passwd,
                          unsigned long clientflag)
{
    return connect_start(uri.host(), usr, passwd, uri.schema(), uri.port(),
                         uri.socket(), clientflag);
}


void
Connection::connect_start(const char *host,
                          const char *user,
                          const char *passwd,
                          const char *db,
                          unsigned int port,
                          const char *unix_socket,
                          unsigned long clientflag)
{
    assert(!_async_status);
    MYSQL *ret;
    _async_status = mysql_real_connect_start(&ret, &mysql, host, user, passwd,
                                            db, port, unix_socket, clientflag);
    if (!_async_status && ret == nullptr) throw_exception();
}


void
Connection::connect_cont(int status)
{
    assert(_async_status);
    MYSQL *ret;
    _async_status = mysql_real_connect_cont(&ret, &mysql, status);
    if (!_async_status && ret == nullptr) throw_exception();
}


ResultSet *
Connection::list_fields_start(const char *table, const char *wild)
{
    assert(!_async_status);
    MYSQL_RES *res;
    _async_status = mysql_list_fields_start(&res, &mysql, table, wild);
    if (_async_status) return nullptr;
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : nullptr;
}


ResultSet *
Connection::list_fields_cont(int status)
{
    assert(_async_status);
    MYSQL_RES *res;
    _async_status = mysql_list_fields_cont(&res, &mysql, status);
    if (_async_status) return nullptr;
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : nullptr;
}


bool
Connection::next_result_start()
{
    assert(!_async_status);
    int ret = 0;
    _async_status = mysql_next_result_start(&ret, &mysql);
    if (!_async_status && 0 < ret) throw_exception();
    return !ret;
}


bool
Connection::next_result_cont(int status)
{
    assert(_async_status);
    int ret = 0;
    _async_status = mysql_next_result_cont(&ret, &mysql, status);
    if (!_async_status && 0 < ret) throw_exception();
    return !ret;
}


void
Connection::query_start(const char *sql)
{
    return query_start(sql, strlen(sql));
}


void
Connection::query_start(const char *sql, unsigned long length)
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_real_query_start(&ret, &mysql, sql, length);
    if (!_async_status && ret) throw_exception();
}


void
Connection::query_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_real_query_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::rollback_start()
{
    assert(!_async_status);
    my_bool ret;
    _async_status = mysql_rollback_start(&ret, &mysql);
    if (!_async_status && ret) throw_exception();
}


void
Connection::rollback_cont(int status)
{
    assert(_async_status);
    my_bool ret;
    _async_status = mysql_rollback_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::select_db_start(const char *db)
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_select_db_start(&ret, &mysql, db);
    if (!_async_status && ret) throw_exception();
}


void
Connection::select_db_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_select_db_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


ResultSet *
Connection::store_result_start()
{
    assert(!_async_status);
    MYSQL_RES *res;
    _async_status = mysql_store_result_start(&res, &mysql);
    if (_async_status) return nullptr;
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : nullptr;
}


ResultSet *
Connection::store_result_cont(int status)
{
    assert(_async_status);
    MYSQL_RES *res;
    _async_status = mysql_store_result_cont(&res, &mysql, status);
    if (_async_status) return nullptr;
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(*this, res) : nullptr;
}


void
Connection::set_character_set_start(const char *csname)
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_set_character_set_start(&ret, &mysql, csname);
    if (!_async_status && ret) throw_exception();
}


void
Connection::set_character_set_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_set_character_set_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::change_user_start(const char *usr, const char *pas, const char *db)
{
    assert(!_async_status);
    my_bool ret;
    _async_status = mysql_change_user_start(&ret, &mysql, usr, pas, db);
    if (!_async_status && ret) throw_exception();
}


void
Connection::change_user_cont(int status)
{
    assert(_async_status);
    my_bool ret;
    _async_status = mysql_change_user_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::send_query_start(const char *sql, unsigned long length)
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_send_query_start(&ret, &mysql, sql, length);
    if (!_async_status && ret) throw_exception();
}


void
Connection::send_query_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_send_query_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::read_query_result_start()
{
    assert(!_async_status);
    my_bool ret;
    _async_status = mysql_read_query_result_start(&ret, &mysql);
    if (!_async_status && ret) throw_exception();
}


void
Connection::read_query_result_cont(int status)
{
    assert(_async_status);
    my_bool ret;
    _async_status = mysql_read_query_result_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::shutdown_start(enum mysql_enum_shutdown_level shutdown_level)
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_shutdown_start(&ret, &mysql, shutdown_level);
    if (!_async_status && ret) throw_exception();
}


void
Connection::shutdown_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_shutdown_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::refresh_start(unsigned int options)
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_refresh_start(&ret, &mysql, options);
    if (!_async_status && ret) throw_exception();
}


void
Connection::refresh_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_refresh_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::kill_start(unsigned long pid)
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_kill_start(&ret, &mysql, pid);
    if (!_async_status && ret) throw_exception();
}


void
Connection::kill_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_kill_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::set_server_option_start(enum enum_mysql_set_option option)
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_set_server_option_start(&ret, &mysql, option);
    if (!_async_status && ret) throw_exception();
}


void
Connection::set_server_option_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_set_server_option_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


void
Connection::ping_start()
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_ping_start(&ret, &mysql);
    if (!_async_status && ret) throw_exception();
}


void
Connection::ping_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_ping_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}


const char *
Connection::stat_start()
{
    assert(!_async_status);
    const char *ret;
    _async_status = mysql_stat_start(&ret, &mysql);
    if (!_async_status && !ret) throw_exception();
    return ret;
}


const char *
Connection::stat_cont(int status)
{
    assert(_async_status);
    const char *ret;
    _async_status = mysql_stat_cont(&ret, &mysql, status);
    if (!_async_status && !ret) throw_exception();
    return ret;
}


void
Connection::reset_connection_start()
{
    assert(!_async_status);
    int ret;
    _async_status = mysql_reset_connection_start(&ret, &mysql);
    if (!_async_status && ret) throw_exception();
}


void
Connection::reset_connection_cont(int status)
{
    assert(_async_status);
    int ret;
    _async_status = mysql_reset_connection_cont(&ret, &mysql, status);
    if (!_async_status && ret) throw_exception();
}

#endif /* MARIADB_VERSION_ID */



}
