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
#ifndef MARIACPP_EXCEPTION_HPP
#define MARIACPP_EXCEPTION_HPP

#include <stdexcept>
#include <iosfwd>

namespace MariaCpp {

class Exception : public std::runtime_error
{
public:
    Exception(const char *str, unsigned number, const char *sqlstate);

    virtual ~Exception() throw () {}

    void print(std::ostream &os) const;

    int errorno() const { return _errno; }

protected:
    Exception(const std::string &reason);
    
private:
    static const unsigned sqlstate_length = 5;

    const int _errno;
    char _sqlstate[sqlstate_length + 1];
};


inline std::ostream &operator<<(std::ostream &os, const Exception &ex)
{
    return ex.print(os), os;
}



class InvalidArgumentException : public Exception
{
public:
    InvalidArgumentException(const std::string &reason) : Exception(reason) {}
};


}
#endif
