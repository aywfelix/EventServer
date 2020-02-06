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
#include <mariacpp/prepared_stmt.hpp>
#include <mariacpp/connection.hpp>
#include <mariacpp/exception.hpp>
#include <mariacpp/resultset.hpp>
#include <mariacpp/time.hpp>
#include <mariacpp/bits/bind.hpp>
#include <cstdlib>
#include <memory>
#include <vector>

#if __cplusplus < 201103L
# define unique_ptr auto_ptr
#endif

namespace MariaCpp {

PreparedStatement::PreparedStatement(Connection &conn)
    : _conn(conn)
    , _stmt(conn.stmt_init())
    , _params()
    , _results()
    , _truncated()
    , _bind_params()
    , _bind_results(true)
{
}


PreparedStatement::~PreparedStatement()
{
    delete [] _params;
    delete [] _results;
    if (_stmt) mysql_stmt_close(_stmt); // No throw even on error (dtor!)
}


void
PreparedStatement::throw_exception()
{
    throw Exception(error_str(), errorno(), sqlstate());
}


void 
PreparedStatement::attr_get(enum enum_stmt_attr_type option, void *arg) const
{
    if (mysql_stmt_attr_get(_stmt, option, arg))
        throw InvalidArgumentException("Unknown option");
}


void 
PreparedStatement::attr_set(enum enum_stmt_attr_type option, const void *arg)
{
    if (mysql_stmt_attr_set(_stmt, option, arg))
        throw InvalidArgumentException("Unknown option");
}


void
PreparedStatement::bind_param(MYSQL_BIND *bind)
{
    _bind_params = false; // Turn off C++ style binding
    if (mysql_stmt_bind_param(_stmt, bind)) throw_exception();
}


void
PreparedStatement::bind_result(MYSQL_BIND *bind)
{
    _bind_results = false; // Turn off C++ style binding
    if (mysql_stmt_bind_result(_stmt, bind)) throw_exception();
}


void
PreparedStatement::close()
{
    // if (!_stmt) return; // is needed?
    my_bool res = mysql_stmt_close(_stmt);
    _stmt = NULL;
    if (res) _conn.throw_exception(); // must throw from _conn! (_stmt invalid)
}


void
PreparedStatement::prepare(const std::string &sql)
{
    assert(!_bind_params && !_params);
    if (mysql_stmt_prepare(_stmt, sql.data(), sql.size())) throw_exception();
    do_reset_bind();
}


void PreparedStatement::do_reset_bind()
{
    _bind_params = false;
    _bind_results = true;
    delete [] _params;
    delete [] _results;
    _params = _results = NULL;
}


void PreparedStatement::reset()
{
    if (mysql_stmt_reset(_stmt)) throw_exception();
    do_reset_bind();
}


void
PreparedStatement::do_bind_params()
{
    assert(_bind_params && _params);
    _bind_params = false;
    const size_t count = param_count();
    if (count) {
        std::vector<MYSQL_BIND> par(count);
        for (unsigned i = 0; i < count; ++i) par[i] = _params[i];
        bind_param(&par[0]);
    }
}


void
PreparedStatement::execute()
{
    if (_bind_params) do_bind_params();
    if (mysql_stmt_execute(_stmt)) throw_exception();
}


void
PreparedStatement::do_bind_results()
{
    assert(_bind_results && !_results);
    _bind_results = false;
    // We will depend on MYSQL_DATA_TRUNCATED status
    my_bool trunc = false;
#   if 50700 <= MYSQL_VERSION_ID
    _conn.get_option(MYSQL_REPORT_DATA_TRUNCATION, &trunc);
#   endif
    if (!trunc) _conn.options(MYSQL_REPORT_DATA_TRUNCATION, &(trunc = true));
    const size_t count = field_count();
    std::unique_ptr<ResultSet> rs(result_metadata());
    if (count && rs.get()) {
        _results = new Bind[count]();
        std::vector<MYSQL_BIND> par(count);
        for (unsigned i = 0; i < count; ++i)
            par[i] = _results[i].init(rs->fetch_field_direct(i));
        bind_result(&par[0]);
    }
}


void
PreparedStatement::do_rebind_results()
{
    // After fetch(), if data was truncated (ie. BIND.error),
    // we will increase buffer size and re-fetch truncated field again.
    const size_t count = field_count();
    if (!count) return;
    assert(_results);
    std::vector<MYSQL_BIND> par(count);
    for (unsigned i = 0; i < count; ++i) {
        if (_results[i].error()) {
            _results[i].realloc(_results[i].raw_length());
            fetch_column(_results[i], i, 0);
        }
        par[i] = _results[i];
    }
    bind_result(&par[0]);
}


bool
PreparedStatement::fetch()
{
    if (_bind_results) do_bind_results();
    int res = mysql_stmt_fetch(_stmt);
    if (1 == res) throw_exception();
    if (MYSQL_NO_DATA == res) return false;
    _truncated =  MYSQL_DATA_TRUNCATED & res;
    if (_truncated && _results) do_rebind_results();
    return true;
}


void
PreparedStatement::fetch_column(Bind &bind,
                                unsigned int column,
                                unsigned long offset)
{
    MYSQL_BIND mybind = bind;
    if (mysql_stmt_fetch_column(_stmt, &mybind, column, offset))
        throw_exception();
}


bool
PreparedStatement::next_result()
{
    int res = mysql_stmt_next_result(_stmt);
    if (0 < res) throw_exception();
    return 0 == res;
}


ResultSet *
PreparedStatement::result_metadata()
{
    MYSQL_RES *res = mysql_stmt_result_metadata(_stmt);
    if (!res && errorno()) throw_exception();
    return res ? new ResultSet(_conn, res) : NULL;
}
    

Bind &
PreparedStatement::param(idx_t col)
{
    const size_t count = param_count();
    if (!_params) {
        _params = new Bind[count]();
        _bind_params = true; // important when all default params set to null!
    }
    assert(col < count);
    if (count <= col) throw InvalidArgumentException("Bind-param out of range");
    return _params[col];
}


const Bind &
PreparedStatement::result(idx_t col) const
{
    const size_t count = field_count();
    assert(_results && col < count);
    if (!_results || count <= col)
        throw InvalidArgumentException("Bind-param out of range");
    return _results[col];
}


void
PreparedStatement::setNull(idx_t col)
{
    if (param(col).setNull()) _bind_params = true;
}
    

void
PreparedStatement::setTinyInt(idx_t col, int8_t value)
{
    if (param(col).setTinyInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setUTinyInt(idx_t col, uint8_t value)
{
    if (param(col).setUTinyInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setSmallInt(idx_t col, int16_t value)
{
    if (param(col).setSmallInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setUSmallInt(idx_t col, uint16_t value)
{
    if (param(col).setUSmallInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setYear(idx_t col, uint16_t value)
{
    if (param(col).setYear(value)) _bind_params = true;
}
    

void
PreparedStatement::setMediumInt(idx_t col, int32_t value)
{
    if (param(col).setMediumInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setUMediumInt(idx_t col, uint32_t value)
{
    if (param(col).setUMediumInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setInt(idx_t col, int32_t value)
{
    if (param(col).setInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setUInt(idx_t col, uint32_t value)
{
    if (param(col).setUInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setBigInt(idx_t col, int64_t value)
{
    if (param(col).setBigInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setUBigInt(idx_t col, uint64_t value)
{
    if (param(col).setUBigInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setBlob(idx_t col, const std::string &value)
{
    if (param(col).setBlob(value)) _bind_params = true;
}
    

void
PreparedStatement::setBoolean(idx_t col, bool value)
{
    if (param(col).setTinyInt(value)) _bind_params = true;
}
    

void
PreparedStatement::setDouble(idx_t col, double value)
{
    if (param(col).setDouble(value)) _bind_params = true;
}
    

void
PreparedStatement::setFloat(idx_t col, float value)
{
    if (param(col).setFloat(value)) _bind_params = true;
}
    

void
PreparedStatement::setString(idx_t col, const char *str)
{
    if (param(col).setCString(str)) _bind_params = true;
}

    
void
PreparedStatement::setString(idx_t col, const std::string &str)
{
    if (param(col).setString(str)) _bind_params = true;
}
    

void
PreparedStatement::setDateTime(idx_t col, const Time &time)
{
    if (param(col).setDateTime(time)) _bind_params = true;
}
    

bool
PreparedStatement::isNull(idx_t col) const
{
    return result(col).isNull();
}
    

std::string
PreparedStatement::getString(idx_t col) const
{
    return result(col).getString();
}
    

int32_t
PreparedStatement::getInt(idx_t col) const
{
    return result(col).getInt();
}
    

uint32_t
PreparedStatement::getUInt(idx_t col) const
{
    return result(col).getUInt();
}
    

int64_t
PreparedStatement::getBigInt(idx_t col) const
{
    return result(col).getBigInt();
}
    

uint64_t
PreparedStatement::getUBigInt(idx_t col) const
{
    return result(col).getUBigInt();
}
    

Time
PreparedStatement::getDate(idx_t col) const
{
    return getDateTime(col);
}


Time
PreparedStatement::getDateTime(idx_t col) const
{
    return result(col).getDateTime();
}
    

Time
PreparedStatement::getTime(idx_t col) const
{
    return getDateTime(col);
}


Time
PreparedStatement::getTimeStamp(idx_t col) const
{
    return getDateTime(col);
}


#ifdef MARIADB_VERSION_ID
//
//  MariaDB-specific non-blocking functions:
//

int
PreparedStatement::async_status() const
{
    return _conn.async_status();
}


bool
PreparedStatement::next_result_start()
{
    assert(!_conn._async_status);
    int ret;
    _conn._async_status = mysql_stmt_next_result_start(&ret, _stmt);
    if (!_conn._async_status && 0 < ret) throw_exception();
    return !ret;
}


bool
PreparedStatement::next_result_cont(int status)
{
    assert(async_status());
    int ret;
    _conn._async_status = mysql_stmt_next_result_cont(&ret, _stmt, status);
    if (!_conn._async_status && 0 < ret) throw_exception();
    return !ret;
}


void
PreparedStatement::prepare_start(const char *query, unsigned long length)
{
    assert(!_conn._async_status);
    int ret;
    _conn._async_status = mysql_stmt_prepare_start(&ret, _stmt, query, length);
    if (!_conn._async_status && ret) throw_exception();
}


void
PreparedStatement::prepare_cont(int status)
{
    assert(_conn._async_status);
    int ret;
    _conn._async_status = mysql_stmt_prepare_cont(&ret, _stmt, status);
    if (!_conn._async_status && ret) throw_exception();
}


void
PreparedStatement::execute_start()
{
    assert(!_conn._async_status);
    if (_bind_params) do_bind_params();
    int ret;
    _conn._async_status = mysql_stmt_execute_start(&ret, _stmt);
    if (!_conn._async_status && ret) throw_exception();
}


void
PreparedStatement::execute_cont(int status)
{
    assert(_conn._async_status);
    int ret;
    _conn._async_status = mysql_stmt_execute_cont(&ret, _stmt, status);
    if (!_conn._async_status && ret) throw_exception();
}


bool
PreparedStatement::fetch_start()
{
    assert(!_conn._async_status);
    if (_bind_results) do_bind_results();
    int ret;
    _conn._async_status = mysql_stmt_fetch_start(&ret, _stmt);
    if (_conn._async_status) return false;
    if (1 == ret) throw_exception();
    if (MYSQL_NO_DATA == ret) return false;
    _truncated =  MYSQL_DATA_TRUNCATED & ret;
    if (_truncated && _results) do_rebind_results();
   return true;
}


bool
PreparedStatement::fetch_cont(int status)
{
    assert(_conn._async_status);
    int ret;
    _conn._async_status = mysql_stmt_fetch_cont(&ret, _stmt, status);
    if (_conn._async_status) return false;
    if (1 == ret) throw_exception();
    if (MYSQL_NO_DATA == ret) return false;
    _truncated =  MYSQL_DATA_TRUNCATED & ret;
    if (_truncated && _results) do_rebind_results();
    return true;
}


void
PreparedStatement::store_result_start()
{
    assert(!_conn._async_status);
    int ret;
    _conn._async_status = mysql_stmt_store_result_start(&ret, _stmt);
    if (!_conn._async_status && ret) throw_exception();
}


void
PreparedStatement::store_result_cont(int status)
{
    assert(_conn._async_status);
    int ret;
    _conn._async_status = mysql_stmt_store_result_cont(&ret, _stmt, status);
    if (!_conn._async_status && ret) throw_exception();
}


void
PreparedStatement::close_start()
{
    assert(!_conn._async_status);
    my_bool ret;
    _conn._async_status = mysql_stmt_close_start(&ret, _stmt);
    if (!_conn._async_status) {
        _stmt = NULL;
        if (ret) _conn.throw_exception();
    }
}


void
PreparedStatement::close_cont(int status)
{
    assert(_conn._async_status);
    my_bool ret;
    _conn._async_status = mysql_stmt_close_cont(&ret, _stmt, status);
    if (!_conn._async_status) {
        _stmt = NULL;
        if (ret) _conn.throw_exception();
    }
}


void
PreparedStatement::reset_start()
{
    assert(!_conn._async_status);
    my_bool ret;
    _conn._async_status = mysql_stmt_reset_start(&ret, _stmt);
    if (!_conn._async_status && ret) throw_exception();
}


void
PreparedStatement::reset_cont(int status)
{
    assert(_conn._async_status);
    my_bool ret;
    _conn._async_status = mysql_stmt_reset_cont(&ret, _stmt, status);
    if (!_conn._async_status && ret) throw_exception();
}


void
PreparedStatement::free_result_start()
{
    assert(!_conn._async_status);
    my_bool ret;
    _conn._async_status = mysql_stmt_free_result_start(&ret, _stmt);
    if (!_conn._async_status && ret) throw_exception();
}


void
PreparedStatement::free_result_cont(int status)
{
    assert(_conn._async_status);
    my_bool ret;
    _conn._async_status = mysql_stmt_free_result_cont(&ret, _stmt, status);
    if (!_conn._async_status && ret) throw_exception();
}



void
PreparedStatement::send_long_data_start(unsigned int param_number,
                                        const char *data, unsigned long len)
{
    assert(!_conn._async_status);
    my_bool ret;
    _conn._async_status = mysql_stmt_send_long_data_start(&ret, _stmt,
                                                          param_number,
                                                          data, len);
    if (!_conn._async_status && ret) throw_exception();
}


void
PreparedStatement::send_long_data_cont(int status)
{
    assert(_conn._async_status);
    my_bool ret;
    _conn._async_status = mysql_stmt_send_long_data_cont(&ret, _stmt, status);
    if (!_conn._async_status && ret) throw_exception();
}

#endif /* MARIADB_VERSION_ID */



}
