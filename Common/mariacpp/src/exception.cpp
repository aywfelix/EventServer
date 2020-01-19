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
#include <mariacpp/exception.hpp>
#include <mysql.h>
#include <cstring>
#include <ostream>

namespace MariaCpp {

Exception::Exception(const char *str, unsigned number, const char *sqlstate)
    : std::runtime_error(str)
    , _errno(number)
{
    memcpy(_sqlstate, sqlstate, sqlstate_length);
    _sqlstate[sqlstate_length] = '\0';
}


Exception::Exception(const std::string &reason)
    : std::runtime_error(reason)
    , _errno(0)
{
    _sqlstate[0] = '\0';
}


void
Exception::print(std::ostream &os) const
{
    if (!_errno && !_sqlstate[0])
        os << "MariaDB lib ERROR: " << what();
    else
        os << "MariaDB SQL ERROR " << _errno << " (" << _sqlstate << ") "
           << what();
}


}
