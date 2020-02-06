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
#include <mariacpp/uri.hpp>
#include <cstdlib>
#include <iostream>


int test(const char *uri, const char *user, const char *passwd)
{
    std::clog << "DB uri: " << uri << std::endl;
    std::clog << "DB user: " << user << std::endl;
    std::clog << "DB passwd: " << passwd << std::endl;

    try {
        my_bool reconnect = 1;
        MariaCpp::Connection conn;

        // Some examples how to modify connection options
        conn.options(MYSQL_OPT_RECONNECT, &reconnect);
        conn.options(MYSQL_INIT_COMMAND,
                     "SET SESSION TRANSACTION ISOLATION LEVEL "
                     "READ UNCOMMITTED");
        conn.options(MYSQL_INIT_COMMAND,
                     "set session wait_timeout = 31536000");
        conn.options(MYSQL_INIT_COMMAND,
                     "set autocommit=1");
        
        // Connect to DB using MySQL Connector/C++ style host URI.
        // You can also use alternative connect() method with
        // the same arguments as C-API.
        conn.connect(MariaCpp::Uri(uri), user, passwd);
        
        std::clog << "Connection status: SUCESS" << std::endl;

        // Print some statistics
        std::clog << "MySQL Stat: " << conn.stat() << std::endl;
        
        conn.close(); // optional
    } catch (MariaCpp::Exception &e) {
        std::cerr << e << std::endl;
        return 1;
    }
    return 0;
}

int main()
{
    // Initialize MariaDB/MySQL library
    // Alternatively call methods:
    //   MariaCpp::library_init()
    //   ....
    //   MariaCpp::library_end()
    // Altough not needed in single-threaded code, it is good practice.
    MariaCpp::scoped_library_init maria_lib_init;

    const char *uri = std::getenv("TEST_DB_URI");
    const char *user = std::getenv("TEST_DB_USER");
    const char *passwd = std::getenv("TEST_DB_PASSWD");
    if (!uri) uri = "tcp://localhost:3306/test";
    if (!user) user = "test";
    if (!passwd) passwd = "";
    
    return test(uri, user, passwd);
}
