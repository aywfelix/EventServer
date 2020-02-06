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
#include <mariacpp/resultset.hpp>
#include <mariacpp/connection.hpp>
#include <mariacpp/exception.hpp>
#include <stdlib.h>

namespace MariaCpp {


void
ResultSet::free_result()
{
    mysql_free_result(_res);
}


MYSQL_ROW
ResultSet::fetch_row()
{
    _lengths = 0;
    _row = mysql_fetch_row(_res);
    if (!_row && _conn.errorno()) _conn.throw_exception();
    return _row;
}

    
std::string
ResultSet::getString(unsigned col) const
{
    assert_col(col);
    const char *data = _row[col];
    if (!data) return std::string();
    unsigned long len = length(col);
    return std::string(data, len);
}


int32_t
ResultSet::getInt(unsigned col) const
{
    assert_col(col);
    if (isNull(col)) return 0;
    if (fetch_field_direct(col)->flags & UNSIGNED_FLAG)
        return strtoul(_row[col], NULL, 10);
    return strtol(_row[col], NULL, 10);
}
    

int64_t
ResultSet::getInt64(unsigned col) const
{
    assert_col(col);
    if (isNull(col)) return 0;
    if (fetch_field_direct(col)->flags & UNSIGNED_FLAG)
        return strtoull(_row[col], NULL, 10);
    return strtoll(_row[col], NULL, 10);
}
    

uint32_t
ResultSet::getUInt(unsigned col) const
{
    assert_col(col);
    if (isNull(col)) return 0;
    if (fetch_field_direct(col)->flags & UNSIGNED_FLAG)
        return strtoul(_row[col], NULL, 10);
    return strtol(_row[col], NULL, 10);
}
    

uint64_t
ResultSet::getUInt64(unsigned col) const
{
    assert_col(col);
    if (isNull(col)) return 0;
    if (fetch_field_direct(col)->flags & UNSIGNED_FLAG)
        return strtoull(_row[col], NULL, 10);
    return strtoll(_row[col], NULL, 10);
}
    

double
ResultSet::getDouble(unsigned col) const
{
    assert_col(col);
    if (isNull(col)) return 0;
    return strtod(_row[col], NULL);
}
    


}
