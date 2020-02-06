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
#include <mariacpp/uri.hpp>
#include <mariacpp/exception.hpp>
#include <stdlib.h>
#include <ostream>

namespace MariaCpp {

static const int DEFAULT_TCP_PORT = 3306;
static const char URI_SOCKET_PRFX[] = "unix://";
static const char URI_PIPE_PRFX[] = "pipe://";
static const char URI_TCP_PRFX[] = "tcp://";

Uri::Uri(const std::string &str)
    : _protocol(PROTOCOL_DEFAULT)
    , _port(DEFAULT_TCP_PORT)
    , _host()
    , _schema()
{
    parse(str);
}


const char *Uri::host() const
{
    return _host.empty() || PROTOCOL_TCP != _protocol
        ? NULL
        : _host == "localhost" ? "127.0.0.1" : _host.c_str();
}


const char *Uri::socket() const
{
    return _host.empty()
        || (PROTOCOL_PIPE != _protocol
            && PROTOCOL_SOCKET != _protocol)
        ? NULL
        : _host.c_str();
}


const char *Uri::schema() const
{
    return _schema.empty() ? NULL : _schema.c_str();
}


void
Uri::set_socket(const std::string &name)
{
    _protocol = PROTOCOL_SOCKET;
    _host = name;
}


void
Uri::set_pipe(const std::string &name)
{
    _protocol = PROTOCOL_PIPE;
    _host = name;
}


void
Uri::set_host(const std::string &host)
{
    _protocol = PROTOCOL_TCP;
    _host = host;
}


void
Uri::parse(const std::string &str)
{
    static const size_t npos = std::string::npos;
    
    if (!str.compare(0, sizeof(URI_SOCKET_PRFX) - 1, URI_SOCKET_PRFX))
        return set_socket(str.substr(sizeof(URI_SOCKET_PRFX) - 1, npos));

    if (!str.compare(0, sizeof(URI_PIPE_PRFX) - 1, URI_PIPE_PRFX))
        return set_pipe(str.substr(sizeof(URI_PIPE_PRFX) - 1, npos));

    std::string rest = str; // prefix tcp:// is optional
    if (!str.compare(0, sizeof(URI_TCP_PRFX) - 1, URI_TCP_PRFX) )
        rest = str.substr(sizeof(URI_TCP_PRFX) - 1, npos);

    if (!rest.empty() && rest[0] == '[') {
        size_t end_sep= rest.find(']');
        if (end_sep == std::string::npos)
            throw InvalidArgumentException("Uri missing closing ']'");

        set_host(rest.substr(1, end_sep - 1));
        rest = rest.substr(end_sep + 1);
    }

    size_t sep = rest.find('/');
    if (sep != std::string::npos) {
        if (rest.size() > sep + 1)
            _schema = rest.substr(sep + 1, rest.size() - sep - 1);        
        rest = rest.substr(0, sep);
    }

    sep = rest.find_last_of(':', std::string::npos);
    if (sep != std::string::npos) {
        _port = atoi(rest.substr(sep + 1, npos).c_str());
        rest = rest.substr(0, sep);
    }
    else _port = DEFAULT_TCP_PORT;

    if (!rest.empty()) set_host(rest);
}


void
Uri::print(std::ostream &os) const
{
    switch(_protocol) {
    case PROTOCOL_SOCKET:
        os << URI_SOCKET_PRFX << _host;
        return;
    case PROTOCOL_PIPE:
        os << URI_PIPE_PRFX << _host;
        return;
    case PROTOCOL_TCP:
        os << URI_TCP_PRFX << _host  << ':' << _port;
        if (!_schema.empty()) os << '/' << _schema;
        return;
    default: break;
    }
}

}
