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
#ifndef MARIACPP_PREPARED_STATEMENT_HPP
#define MARIACPP_PREPARED_STATEMENT_HPP

#include <stdint.h>
#include <mysql.h>
#include <string>

namespace MariaCpp {

class Connection;
class Bind;
class ResultSet;
struct Time;

class PreparedStatement
{
public:
    typedef unsigned int idx_t;
    
    PreparedStatement(Connection &conn, MYSQL_STMT *stmt);

    ~PreparedStatement(); // like close() but cannot throw

    my_ulonglong affected_rows() const
        { return mysql_stmt_affected_rows(_stmt); }

    void attr_get(enum enum_stmt_attr_type option, void *arg) const;

    void attr_set(enum enum_stmt_attr_type option, const void *arg);

    // Optional C-style param binding
    void bind_param(MYSQL_BIND *bind);

    // Optional C-style result binding
    void bind_result(MYSQL_BIND *bind);

    // After close() this object cannot be used any longer
    void close(); // can throw

    void data_seek(my_ulonglong offset)
        { return mysql_stmt_data_seek(_stmt, offset); }

    unsigned int errorno() const
        { return mysql_stmt_errno(_stmt); }

    const char *error_str() const
        { return mysql_stmt_error(_stmt); }

    void execute();

    // Info: truncated status must be enabled earlier using mysql_options()
    bool fetch(bool *truncated = 0);

    void fetch_column(Bind &bind, unsigned int column, unsigned long offset);

    unsigned int field_count() const
        { return mysql_stmt_field_count(_stmt); }
    
    my_ulonglong insert_id() const
        { return mysql_stmt_insert_id(_stmt); }

    bool next_result();

    my_ulonglong num_rows() const
        { return mysql_stmt_num_rows(_stmt); }

    unsigned long param_count() const
        { return mysql_stmt_param_count(_stmt); }
    
    // In most cases don't call prepare() directly!
    // Use: Connection::prepare(const std::string &)
    void prepare(const std::string &sql);

    void reset();

    ResultSet *result_metadata();
    
    MYSQL_ROW_OFFSET row_seek(MYSQL_ROW_OFFSET offset)
        { return mysql_stmt_row_seek(_stmt, offset); }
    
    MYSQL_ROW_OFFSET row_tell() const
        { return mysql_stmt_row_tell(_stmt); }

    void send_long_data(idx_t col, const char *data, unsigned long length)
        { if (mysql_stmt_send_long_data(_stmt, col, data, length))
                throw_exception(); }
    
    const char *sqlstate() const
        { return mysql_stmt_sqlstate(_stmt); }

    void store_result()
        { if (mysql_stmt_store_result(_stmt)) throw_exception(); }
    
    // Methods for Connector/C++ style param binding

    void setNull(idx_t col);

    void setTinyInt(idx_t col, int8_t value);
    
    void setUTinyInt(idx_t col, uint8_t value);
    
    void setSmallInt(idx_t col, int16_t value);
    
    void setUSmallInt(idx_t col, uint16_t value);
    
    void setYear(idx_t col, uint16_t value);
    
    void setMediumInt(idx_t col, int32_t value);
    
    void setUMediumInt(idx_t col, uint32_t value);
    
    void setInt(idx_t col, int32_t value);
    
    void setUInt(idx_t col, uint32_t value);
    
    void setBigInt(idx_t col, int64_t value);
    
    void setUBigInt(idx_t col, uint64_t value);

    void setBlob(idx_t col, const std::string &v);
    
    void setBoolean(idx_t col, bool value);
    
    void setDouble(idx_t col, double value);

    void setFloat(idx_t col, float value);

    // This is special: str==NULL is allowed
    void setString(idx_t col, const char *str);
    
    void setString(idx_t col, const std::string &str);
    
    void setBinary(idx_t col, const std::string &v)
        { return setBlob(col, v); }
    
    void setChar(idx_t col, char c)
        { return setString(col, std::string(1, c)); }

    void setChar(idx_t col, const std::string &v)
        { return setString(col, v); }

    void setText(idx_t col, const std::string &v)
        { return setString(col, v); }
    
    void setVarBinary(idx_t col, const std::string &v)
        { return setBlob(col, v); }
    
    void setVarChar(idx_t col, const std::string &v)
        { return setString(col, v); }
    
    void setDate(idx_t col, const Time &time)
        { return setDateTime(col, time); }

    void setDateTime(idx_t col, const Time &time);

    void setTime(idx_t col, const Time &time)
        { return setDateTime(col, time); }

    void setTimeStamp(idx_t col, const Time &time)
        { return setDateTime(col, time); }

    bool isNull(idx_t col) const;

    std::string getString(idx_t col) const;

    std::string getBinary(idx_t col) const
        { return getString(col); }

    int32_t getInt(idx_t col) const;

    uint32_t getUInt(idx_t col) const;

    int64_t getBigInt(idx_t col) const;

    uint64_t getUBigInt(idx_t col) const;

    Time getDate(idx_t col) const; // same as getDateTime(col)

    Time getDateTime(idx_t col) const;

    Time getTime(idx_t col) const; // same as getDateTime(col)

    Time getTimeStamp(idx_t col) const; // same as getDateTime(col)

private:
    // Noncopyable
    PreparedStatement(const PreparedStatement &);
    void operator=(PreparedStatement &);

    // Implicit convertion from char* to std::string not allowed
    // Please use setCString(idx_t, const char *) instead!
    // void setString(idx_t col, const char *str);

    void throw_exception();

    inline Bind &param(idx_t col);

    inline const Bind &result(idx_t col) const;

    Connection &_conn;
    MYSQL_STMT *_stmt;
    Bind *_params;
    Bind *_results;
    bool _bind_params; // C++ style binding
    bool _bind_results;
};


}
#endif
