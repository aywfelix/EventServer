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
#include <mysql.h>
#include <cassert>
#include <cstddef>

namespace MariaCpp {


// unsigned long
// escape_string(char *to, const char *from, unsigned long length)
// {
//     return mysql_escape_string(to, from, length);
// }


const char *
get_client_info()
{
    return mysql_get_client_info();
}


unsigned long
get_client_version()
{
    return mysql_get_client_version();
}


unsigned long
hex_string(char *to, const char *from, unsigned long length)
{
    assert(!"NOT IMPLEMENTED YET");
    return 0;
    // return mysql_hex_string(to, from, length);
}


void
library_end()
{
    return mysql_library_end();
}


int
library_init(int argc, char **argv, char **groups)
{
    return mysql_library_init(argc, argv, groups);
}


int
library_init()
{
    return mysql_library_init(0, NULL, NULL);
}


unsigned int
thread_safe()
{
    return mysql_thread_safe();
}


void
thread_end()
{
    return mysql_thread_end();
}


bool
thread_init()
{
    return mysql_thread_init();
}



}
