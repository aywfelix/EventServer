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
#include <mariacpp/prepared_stmt.hpp>
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

    try {
        MariaCpp::Connection conn;
        conn.connect(MariaCpp::Uri(uri), user, passwd);
        conn.autocommit(true);
        std::clog << "Connected." << std::endl;
        
        conn.query("CREATE TEMPORARY TABLE test"
                    "(id INT, label CHAR(15), d DATE)");
        std::clog << "Temporary table created." << std::endl;

        MariaCpp::PreparedStatement stmt(conn);
        stmt.prepare("INSERT INTO test (id,label,d) values(?,?,?)");

        // C-style param binding
        assert(3 == stmt.param_count());
        int data_id = 1;
        char data_label = 'a';
        MYSQL_TIME data_d = MYSQL_TIME();
        my_bool null_d = true;
        MYSQL_BIND bind[3] = {0};
        bind[0].buffer_type = MYSQL_TYPE_LONG;
        bind[0].buffer = (char *)&data_id;
        // bind[0].buffer_length = 4;
        bind[1].buffer_type = MYSQL_TYPE_STRING;
        bind[1].buffer = &data_label;
        bind[1].buffer_length = 1;
        bind[2].buffer_type = MYSQL_TYPE_DATE;
        bind[2].buffer = (char *)&data_d;
        // bind[2].buffer_length = sizeof(MYSQL_TIME);
        bind[2].is_null = &null_d;
        stmt.bind_param(bind);
        stmt.execute();
        
        data_id = 2;
        data_label = 'b';
        stmt.execute();

        data_id = 3;
        data_label = 'c';
        data_d.year = 2015;
        data_d.month = 8;
        data_d.day = 24;
        null_d = false;
        stmt.execute();

        data_id = 4;
        data_label = 'd';
        data_d.year = 2016;
        data_d.month = 1;
        stmt.execute();
        std::clog << "Insert done." << std::endl;
        
        // Select results using C-style result binding        
        std::clog << "Selecting from DB:" << std::endl;

        stmt.prepare("SELECT id, label, d FROM test ORDER BY id");
        stmt.bind_result(bind);
        stmt.execute();
        while (stmt.fetch()) {
            std::cout << "id = " << data_id
                      << ", label = '" << data_label << "'"
                      << ", date = ";
            if (null_d) std::cout << "NULL";
            else std::cout << data_d.year << '-' << data_d.month
                           << '-' << data_d.day;
            std::cout << std::endl;
        }

        conn.query("DROP TEMPORARY TABLE IF EXISTS test");

        // conn.close(); // optional
    } catch (MariaCpp::Exception &e) {
        std::cerr << e << std::endl;
        return 1;
    }
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
