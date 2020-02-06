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
#ifndef MARIACPP_CONNECTION_HPP
#define MARIACPP_CONNECTION_HPP

#include <mysql.h>
#include <cassert>
#include <string>

namespace MariaCpp {

class PreparedStatement;
class ResultSet;
class Uri;

class Connection
{
public:
    Connection();

    ~Connection();

    unsigned long affected_rows()
        { return mysql_affected_rows(&mysql); }

    void autocommit(bool mode)
        { CC(); if (mysql_autocommit(&mysql, mode)) throw_exception(); }


    void change_user(const char *usr, const char *pas, const char *db)
        { CC(); if(mysql_change_user(&mysql, usr, pas, db)) throw_exception(); }

    const char *character_set_name()
        { return mysql_character_set_name(&mysql); }

    void close();

    void commit()
        { CC(); if (mysql_commit(&mysql)) throw_exception(); }

    // mysql_connect() is depricated.
    // In MariaCpp: connect() is an alias for real_connect().
    void connect(const Uri &uri, const char *usr, const char *passwd,
                 unsigned long clientflag = 0);

    void connect(const char *host,
                 const char *user,
                 const char *passwd,
                 const char *db,
                 unsigned int port,
                 const char *unix_socket,
                 unsigned long clientflag);

    void dump_debug_info()
        { CC(); if (mysql_dump_debug_info(&mysql)) throw_exception(); }

    unsigned int errorno()
        { return mysql_errno(&mysql); }

    const char *error_str()
        { return mysql_error(&mysql); }

    // mysql_escape_string() is forbidden.
    // This method is using mysq_real_escape_string()
    unsigned long escape_string(char *to, const char *from,
                                     unsigned long length)
        { return mysql_real_escape_string(&mysql, to, from, length); }

    std::string escape_string(const std::string &str);

    unsigned int field_count()
        { return mysql_field_count(&mysql); }

    void get_character_set_info(MY_CHARSET_INFO *cs)
        { return mysql_get_character_set_info(&mysql, cs); }

    const char *get_host_info()
        { return mysql_get_host_info(&mysql); }

#   if 50700 <= MYSQL_VERSION_ID
    void get_option(enum mysql_option option, void *arg)
        { if (mysql_get_option(&mysql, option, arg)) throw_exception(); }
#   endif

    unsigned int get_proto_info()
        { return mysql_get_proto_info(&mysql); }

    const char *get_server_info()
        { return mysql_get_server_info(&mysql); }

    unsigned long get_server_version()
        { return mysql_get_server_version(&mysql); }

    const char *get_ssl_cipher()
        { return mysql_get_ssl_cipher(&mysql); }

    const char *info()
        { return mysql_info(&mysql); }

    my_ulonglong insert_id()
        { return mysql_insert_id(&mysql); }

    void kill(unsigned long pid)
        { CC(); if (mysql_kill(&mysql, pid)) throw_exception(); }

    ResultSet *list_dbs(const char *wild);

    ResultSet *list_fields(const char *table, const char *wild);

    ResultSet *list_processes();

    ResultSet *list_tables(const char *wild);

    bool more_results()
        { return mysql_more_results(&mysql); }

    // Caution: result meaning differs from mysql_next_result()
    bool next_result();

    void options(enum mysql_option option, const void *arg);

#   if 50600 <= MYSQL_VERSION_ID
    void options4(enum mysql_option option, const void *arg1, const void *arg2);
#   endif

    void ping()
        { CC(); if (mysql_ping(&mysql)) throw_exception(); }

    PreparedStatement *prepare(const std::string &sql);

    void query(const char *sql)
        { CC(); if (mysql_query(&mysql, sql)) throw_exception(); }

    void query(const char *sql, unsigned long length)
        { CC();
          if (mysql_real_query(&mysql, sql, length)) throw_exception(); }

    void query(const std::string &sql)
        { return query(sql.data(), sql.size()); }

    // In MariaCpp: real_escape_string() == escape_string()
    unsigned long real_escape_string(char *to, const char *from,
                                     unsigned long length)
        { return mysql_real_escape_string(&mysql, to, from, length); }

    std::string real_escape_string(const std::string &str)
        { return escape_string(str); }

    // In MariaCpp: real_connect() == connect()
    void real_connect(const Uri &uri, const char *usr, const char *passwd,
                      unsigned long clientflag = 0)
        { return connect(uri, usr, passwd, clientflag); }

    void real_connect(const char *host,
                 const char *user,
                 const char *passwd,
                 const char *db,
                 unsigned int port,
                 const char *unix_socket,
                 unsigned long cliflag)
        { return connect(host, user, passwd, db, port, unix_socket, cliflag); }

    // In MariaCpp: real_query(sql, length) == query(sql, length)
    void real_query(const char *sql, unsigned long length)
        { return query(sql, length); }

    void refresh(unsigned int options)
        { CC(); if (mysql_refresh(&mysql, options)) throw_exception();}

    void reload()
        { CC(); if (mysql_reload(&mysql)) throw_exception(); }

#   if 50700 <= MYSQL_VERSION_ID
    void reset_connection()
        { if (mysql_reset_connection(&mysql)) throw_exception(); }
#   endif

    void rollback()
        { CC(); if (mysql_rollback(&mysql)) throw_exception(); }

    void select_db(const char *db)
        { CC(); if (mysql_select_db(&mysql, db)) throw_exception(); }

#   if 50700 <= MYSQL_VERSION_ID
    std::string session_track_get_first(enum enum_session_state_type type);

    // Returns true on success
    bool session_track_get_next(enum enum_session_state_type type,
                                std::string &res);
#   endif

    void set_character_set(const char *csname)
        { CC(); if (mysql_set_character_set(&mysql, csname)) throw_exception();}

    void set_local_infile_default()
        { return mysql_set_local_infile_default(&mysql); }

    void set_local_infile_handler(
        int (*local_infile_init)(void **, const char *, void *),
        int (*local_infile_read)(void *, char *, unsigned int),
        void (*local_infile_end)(void *),
        int (*local_infile_error)(void *, char*, unsigned int),
        void *userdata);

    void set_server_option(enum enum_mysql_set_option option)
        { CC(); if (mysql_set_server_option(&mysql, option)) throw_exception();}

    void shutdown(enum mysql_enum_shutdown_level shutdown_level)
        { CC(); if (mysql_shutdown(&mysql, shutdown_level)) throw_exception(); }

    const char *sqlstate()
        { return mysql_sqlstate(&mysql); }

    bool ssl_set(const char *key, const char *cert, const char *ca,
                 const char *capath, const char *cipher)
        { return mysql_ssl_set(&mysql, key, cert, ca, capath, cipher); }

    const char *stat();

    // Don't call stmt_init() directly.
    // Instead use: prepare(sql) or new PreparedStatment(*this)
    MYSQL_STMT *stmt_init();

    ResultSet *store_result();

    unsigned long thread_id()
        { return mysql_thread_id(&mysql); }

    ResultSet *use_result();

    unsigned int warning_count()
        { return mysql_warning_count(&mysql); }

    void throw_exception();

#   ifdef MARIADB_VERSION_ID
    //
    //  MariaDB-specific functions:
    //
    //
    //  MariaDB-specific non-blocking functions:
    //  https://mariadb.com/kb/en/library/non-blocking-client-library/
    //
    //  MariaDB/C-library functions _start()/_cont() return ready_status.
    //  In MariaCPP library slightly other aproach is used:
    //  All non-blocking methods return the same type as their
    //  blocking counterpart, and async_status() is stored inside this object.
    //  After each call to _start()/_cont() function, the very first thing
    //  you have to do is to call async_status().
    //  1. if (async_status()=0) you can use result of given method
    //  2. otherwise you have to loop over async_wait()&_cont(status) methods
    //     until async_status() will become 0 (or exception will be thrown).
    //  Sample implementation connect:
    //     conn.connect_start(MariaCpp::Uri(uri), user, passwd);
    //     while (conn.async_status()) conn.connect_cont(conn.async_wait());
    //
    int async_status() const { return _async_status; }

    // This is only sample implementation of async poll method.
    int async_wait();

    void autocommit_start(bool mode);

    void autocommit_cont(int status);

    void close_start()
        { _async_status = mysql_close_start(&mysql); }

    void close_cont(int status)
        { _async_status = mysql_close_cont(&mysql, status); }

    void commit_start();

    void commit_cont(int status);

    void dump_debug_info_start();

    void dump_debug_info_cont(int status);

    void connect_start(const Uri &uri, const char *usr, const char *passwd,
                       unsigned long clientflag = 0);

    void connect_start(const char *host,
                       const char *user,
                       const char *passwd,
                       const char *db,
                       unsigned int port,
                       const char *unix_socket,
                       unsigned long clientflag);

    void connect_cont(int status);

    my_socket get_socket()
        { return mysql_get_socket(&mysql); }

    unsigned int get_timeout_value() const
        { return mysql_get_timeout_value(&mysql); }

    unsigned int get_timeout_value_ms() const
        { return mysql_get_timeout_value_ms(&mysql); }

    ResultSet *list_fields_start(const char *table, const char *wild);

    ResultSet *list_fields_cont(int status);

    // Caution: result meaning differs from mysql_next_result()
    bool next_result_start();

    // Caution: result meaning differs from mysql_next_result()
    bool next_result_cont(int status);

    void query_start(const char *sql);

    void query_start(const char *sql, unsigned long length);

    void query_start(const std::string &sql)
        { return query_start(sql.data(), sql.size()); }

    void query_cont(int status);

    void rollback_start();

    void rollback_cont(int status);

    void select_db_start(const char *db);

    void select_db_cont(int status);

    ResultSet *store_result_start();

    ResultSet *store_result_cont(int status);

    void set_character_set_start(const char *db);

    void set_character_set_cont(int status);

    void change_user_start(const char *usr, const char *pas, const char *db);

    void change_user_cont(int status);

    void send_query_start(const char *sql, unsigned long length);

    void send_query_cont(int status);

    void read_query_result_start();

    void read_query_result_cont(int status);

    void shutdown_start(enum mysql_enum_shutdown_level shutdown_level);

    void shutdown_cont(int status);

    void refresh_start(unsigned int options);

    void refresh_cont(int status);

    void kill_start(unsigned long pid);

    void kill_cont(int status);

    void set_server_option_start(enum enum_mysql_set_option option);

    void set_server_option_cont(int status);

    void ping_start();

    void ping_cont(int status);

    const char *stat_start();

    const char *stat_cont(int status);

    void reset_connection_start();

    void reset_connection_cont(int status);
#   endif

private:
    // Noncopyable
    Connection(const Connection &);
    void operator=(Connection &);

    inline void CC();

    MYSQL mysql;
#   ifdef MARIADB_VERSION_ID
    friend class PreparedStatement;
    friend class ResultSet;
    int _async_status;
#   endif
};



// Check if connect() was called before
void
Connection::CC()
{
    assert(mysql.methods);
}



}
#endif
