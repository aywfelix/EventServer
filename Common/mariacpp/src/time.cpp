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
#include <mariacpp/time.hpp>
#include <ostream>

extern "C" {
size_t mariacpp_time_to_string(const MYSQL_TIME *tm, char *time_str,size_t len);
my_bool mariacpp_str_to_TIME(const char *str, size_t length, MYSQL_TIME *tm);
}

namespace MariaCpp {


Time::Time(const std::string &str)
    : MYSQL_TIME()
{
    if (mariacpp_str_to_TIME(str.data(), str.length(), this))
        time_type = MYSQL_TIMESTAMP_ERROR;
}

Time
Time::none()
{
    MYSQL_TIME tm = MYSQL_TIME();
    tm.time_type = MYSQL_TIMESTAMP_NONE;
    return tm;
}


Time
Time::date(year_t year, month_t month, day_t day)
{
    MYSQL_TIME tm = MYSQL_TIME();
    tm.year = year;
    tm.month = month;
    tm.day = day;
    tm.time_type = MYSQL_TIMESTAMP_DATE;
    return tm;
}


Time
Time::datetime(year_t year, month_t month, day_t day,
               hour_t hour, minute_t minute, second_t second)
{
    MYSQL_TIME tm = MYSQL_TIME();
    tm.year = year;
    tm.month = month;
    tm.day = day;
    tm.hour = hour;
    tm.minute = minute;
    tm.second = second;
    tm.time_type = MYSQL_TIMESTAMP_DATETIME;
    return tm;
}


Time
Time::time(hour_t hour, minute_t minute, second_t second)
{
    MYSQL_TIME tm = MYSQL_TIME();
    tm.hour = hour;
    tm.minute = minute;
    tm.second = second;
    tm.time_type = MYSQL_TIMESTAMP_TIME;
    return tm;
}


void
Time::print(std::ostream &os) const
{
    char buff[40];
    size_t len = mariacpp_time_to_string(this, buff, sizeof(buff)-1);
    buff[len] = '\0';
    os << buff;
}

}
