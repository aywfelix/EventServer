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
#include <mariacpp/time.hpp>
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
                    "(id INT, label CHAR(30), d DATE)");
        std::clog << "Temporary table created." << std::endl;

        std::unique_ptr<MariaCpp::PreparedStatement> stmt(
            conn.prepare("INSERT INTO test (id,label,d) values(?,?,?)"));

        // C++-style param binding
        assert(3 == stmt->param_count());
        stmt->setInt(0, 1);
        stmt->setString(1, "a");
        stmt->execute();

        stmt->setInt(0, 2);
        stmt->setString(1, std::string("b 12345678901234567890"));
        stmt->execute();

        stmt->setInt(0, 3);
        stmt->setString(1, "c 12345678901234567890123");
        stmt->setTime(2, MariaCpp::Time("2015-08-24"));
        stmt->execute();

        stmt->setInt(0, 4);
        stmt->setString(1, NULL);
        // stmt->setNull(1);
        stmt->setTime(2, MariaCpp::Time("2016-01-22"));
        stmt->execute();

        
       // Select results using C-style result binding        
        std::clog << "Selecting from DB:" << std::endl;
        stmt.reset(conn.prepare("SELECT id, label, d FROM test ORDER BY id"));
        stmt->execute();
        while (stmt->fetch()) {
            std::cout << "id = " << stmt->getInt(0) << ", label = ";
            if (stmt->isNull(1)) std::cout << "NULL";
            else std::cout << "'" << stmt->getString(1) << "'";
            std::cout << ", date = ";
            if (stmt->isNull(2)) std::cout << "NULL";
            else std::cout << stmt->getTime(2) ;
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
