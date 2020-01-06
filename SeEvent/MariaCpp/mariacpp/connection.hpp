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
    
    bool connect(const Uri &uri, const char *usr, const char *passwd,
                 unsigned long clientflag = 0);

    bool connect(const char *host,
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

    unsigned int field_count()
        { return mysql_field_count(&mysql); }

    void get_character_set_info(MY_CHARSET_INFO *cs)
        { return mysql_get_character_set_info(&mysql, cs); }

    const char *get_host_info()
        { return mysql_get_host_info(&mysql); }

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

    void ping()
        { CC(); if (mysql_ping(&mysql)) throw_exception(); }

	int MyPing()
	{
		CC();
		return mysql_ping(&mysql);
	}

    PreparedStatement *prepare(const std::string &sql);
    
    void query(const char *sql)
        { CC(); if (mysql_query(&mysql, sql)) throw_exception(); }

    void query(const char *sql, unsigned long length)
        { CC();
          if (mysql_real_query(&mysql, sql, length)) throw_exception(); }

    void query(const std::string &sql)
        { return query(sql.data(), sql.size()); }
    
    unsigned long escape_string(char *to, const char *from,
                                     unsigned long length)
        { return mysql_real_escape_string(&mysql, to, from, length); }

    std::string escape_string(const std::string &str);

    void refresh(unsigned int options)
        { CC(); if (mysql_refresh(&mysql, options)) throw_exception();}
    
    void reload()
        { CC(); if (mysql_reload(&mysql)) throw_exception(); }

    bool rollback()
        { return mysql_rollback(&mysql); }

    void select_db(const char *db)
        { CC(); if (mysql_select_db(&mysql, db)) throw_exception(); }

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

    PreparedStatement *stmt_init();
    
    ResultSet *store_result();

    unsigned long thread_id()
        { return mysql_thread_id(&mysql); }

    ResultSet *use_result();

    unsigned int warning_count()
        { return mysql_warning_count(&mysql); }

    void throw_exception();

private:
    // Noncopyable
    Connection(const Connection &);
    void operator=(Connection &);

    inline void CC();

    MYSQL mysql;
};



// Check if connect() was called before
void
Connection::CC()
{
    assert(mysql.methods);
}



}
#endif
