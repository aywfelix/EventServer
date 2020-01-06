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
#ifndef MARIACPP_URI_HPP
#define MARIACPP_URI_HPP

#include <string>

namespace MariaCpp {

class Uri
{
public:
    enum Protocol // == mysql_protocol_type
    {
        PROTOCOL_DEFAULT, PROTOCOL_TCP, PROTOCOL_SOCKET,
        PROTOCOL_PIPE, PROTOCOL_MEMORY
    };
    
    Uri(const std::string &str);

    int port() const { return _port; }

    Protocol protocol() const { return _protocol; }

    const char *host() const;

    const char *socket() const;

    const char *schema() const;

    void set_schema(const std::string &schema) { _schema = schema; }

    void print(std::ostream &os) const;

private:
    void parse(const std::string &str);

    void set_socket(const std::string &name);
    
    void set_pipe(const std::string &name);
    
    void set_host(const std::string &host);
    
    Protocol _protocol;
    int _port;
    std::string _host;
    std::string _schema;
};

inline std::ostream &operator<<(std::ostream &os, const Uri &uri)
{
    return uri.print(os), os;
}



}
#endif
