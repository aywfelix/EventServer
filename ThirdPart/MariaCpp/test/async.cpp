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
#include <mariacpp/lib.hpp>
#include <mariacpp/connection.hpp>
#include <mariacpp/exception.hpp>
#include <mariacpp/resultset.hpp>
#include <mariacpp/uri.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>

#if __cplusplus < 201103L
# define unique_ptr auto_ptr
#endif


int test(const char *uri, const char *user, const char *passwd)
{
    std::clog << "DB uri: " << uri << std::endl;
    std::clog << "DB user: " << user << std::endl;
    std::clog << "DB passwd: " << passwd << std::endl;

#   ifdef MARIADB_VERSION_ID
    try {
        MariaCpp::Connection conn;
        unsigned default_timeout= 3;
        conn.options(MYSQL_OPT_NONBLOCK, 0);
        conn.options(MYSQL_OPT_CONNECT_TIMEOUT, &default_timeout);
        conn.options(MYSQL_OPT_READ_TIMEOUT, &default_timeout);
        conn.options(MYSQL_OPT_WRITE_TIMEOUT, &default_timeout);

        conn.connect_start(MariaCpp::Uri(uri), user, passwd);
        while (conn.async_status()) conn.connect_cont(conn.async_wait());
        std::clog << "Connected." << std::endl;
        conn.autocommit_start(true);
        while (conn.async_status()) conn.autocommit_cont(conn.async_wait());

        conn.query_start("CREATE TEMPORARY TABLE test"
                         "(id INT, label CHAR(15), d DATETIME(3))");
        while (conn.async_status()) conn.query_cont(conn.async_wait());
        std::clog << "Temporary table created." << std::endl;

        conn.query_start("INSERT INTO test(id, label, d) VALUES"
                         " (1,'a', NULL)"
                         ",(2,'b', FROM_UNIXTIME(1575067490.234))"
                         ",(3, 'c', NOW(3))");
        while (conn.async_status()) conn.query_cont(conn.async_wait());
        std::clog << "Sample data inserted" << std::endl;

        std::clog << "Selecting from DB:" << std::endl;
        conn.query_start("SELECT id, label, d FROM test ORDER BY id ASC");
        while (conn.async_status()) conn.query_cont(conn.async_wait());

        // After invoking SELECT query, you must use {store/use}_result()
        std::unique_ptr<MariaCpp::ResultSet> res(conn.store_result_start());
        while (conn.async_status())
            res.reset(conn.store_result_cont(conn.async_wait()));
        // next() is an alias for fetch_row()
        if (res.get()) {
            bool next = res->next_start();
            while (res->async_status())
                next = res->next_cont(conn.async_wait());
            while (next) {
                std::cout << "id = " << res->getInt(0)
                          << ", label = '" << res->getRaw(1)<< "'"
                          << ", date = "
                          << (res->isNull(2) ?"NULL":res->getString(2).c_str())
                          << std::endl;
                next = res->next_start();
                while (res->async_status())
                    next = res->next_cont(conn.async_wait());
            }
        }
        res.reset();

        conn.query_start("DROP TEMPORARY TABLE IF EXISTS test");
        while (conn.async_status()) conn.query_cont(conn.async_wait());

        // conn.close(); // optional
    } catch (MariaCpp::Exception &e) {
        std::cerr << e << std::endl;
        return 1;
    }
#   endif
    return 0;
}


int main()
{
    MariaCpp::scoped_library_init maria_lib_init;

    const char *uri = std::getenv("TEST_DB_URI");
    const char *user = std::getenv("TEST_DB_USER");
    const char *passwd = std::getenv("TEST_DB_PASSWD");
    if (!uri) uri = "tcp://localhost:3306/test";
    if (!user) user = "test";
    if (!passwd) passwd = "";

    return test(uri, user, passwd);
}
