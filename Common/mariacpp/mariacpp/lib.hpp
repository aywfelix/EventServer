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
#ifndef MARIACPP_LIB_HPP
#define MARIACPP_LIB_HPP

namespace MariaCpp {

// DEPRICATED: use real_escape_string()
// unsigned long escape_string(char *to, const char *from, unsigned long len);

const char *get_client_info();

unsigned long get_client_version();

unsigned long hex_string(char *to, const char *from, unsigned long length);

void library_end();

int library_init(int argc, char **argv, char **groups);

int library_init();

unsigned int thread_safe();

void thread_end();

bool thread_init();


struct scoped_library_init
{
    scoped_library_init() { library_init(); }
    ~scoped_library_init() { library_end(); }
};


struct scoped_thread_init
{
    scoped_thread_init() { thread_init(); }
    ~scoped_thread_init() { thread_end(); }
};


}
#endif
