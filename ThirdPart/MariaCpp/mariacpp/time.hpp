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
#ifndef MARIACPP_TIME_HPP
#define MARIACPP_TIME_HPP

#include <mysql.h>
#include <iosfwd>
#include <string>

namespace MariaCpp {

struct Time : public MYSQL_TIME
{
    typedef unsigned year_t;
    typedef unsigned month_t;
    typedef unsigned day_t;
    typedef unsigned hour_t;
    typedef unsigned minute_t;
    typedef unsigned second_t;

    Time(const MYSQL_TIME &time) : MYSQL_TIME(time) {}

    Time(const std::string &str);

    static Time none();

    static Time date(year_t year, month_t month, day_t day);

    static Time time(hour_t hour, minute_t minute, second_t second);

    static Time datetime(year_t year, month_t month, day_t day,
                         hour_t hour, minute_t minute, second_t second);

    void print(std::ostream &os) const;
};


inline std::ostream &operator<<(std::ostream &os, const Time &time)
{
    return time.print(os), os;
}


}
#endif
