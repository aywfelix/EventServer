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
#include <mariacpp/time.hpp>
#include <mariacpp/uri.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>

#if __cplusplus < 201103L
# define unique_ptr auto_ptr
#endif


using namespace MariaCpp;

const char *uri = "tcp://localhost:3306/test";
const char *user = "test";
const char *passwd = "";

void get_env() {
    const char *env = std::getenv("TEST_DB_URI");
    if (env) uri = env;
    env = std::getenv("TEST_DB_USER");
    if (env) user = env;
    env = std::getenv("TEST_DB_PASSWD");
    if (env) passwd = env;
    
}

int main()
{
    scoped_library_init maria_lib_init;
    get_env();

    try {
        Connection conn;
        conn.connect(Uri(uri), user, passwd);
        
        conn.query("CREATE TEMPORARY TABLE test "
                    "(i INT, s CHAR(15), d DATETIME)");
        std::unique_ptr<PreparedStatement> stmt(
            conn.prepare("INSERT INTO test (i,s,d) values(?,?,?)"));

        assert(3 == stmt->param_count());
        stmt->setInt(0, 1);
        stmt->setString(1, "string-1");
        stmt->setDateTime(2, Time("2016-03-23 02:41"));
        stmt->execute();

        stmt->setInt(0, 2);
        stmt->setNull(1);
        stmt->setDateTime(2, Time::datetime(2015, 02, 21, 12, 45, 51));
        stmt->execute();

        stmt.reset(conn.prepare("SELECT i, s, d FROM test ORDER BY i ASC"));
        stmt->execute();
        while (stmt->fetch()) {
            std::cout << "i = " << stmt->getInt(0);
            std::cout << ", s = ";
            if (stmt->isNull(1)) std::cout << "NULL";
            else std::cout << stmt->getString(1);
            std::cout << ", d = " << stmt->getTime(2) ;
            std::cout << std::endl;
        }
        conn.close();
    } catch (Exception &e) {
        std::cerr << e << std::endl;
        return 1;
    }
    return 0;
}
