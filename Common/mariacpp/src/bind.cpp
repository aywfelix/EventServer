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
#include <mariacpp/bits/bind.hpp>
#include <mariacpp/exception.hpp>
#include <mariacpp/time.hpp>
#include <errmsg.h> // MariaDB
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <sstream>

namespace MariaCpp {


void *
Bind::Buffer::data(const bool &heap) const
{
    return heap ? _data : (void *)this;
}


size_t
Bind::Buffer::size(const bool &heap) const
{
    return heap ? _size : sizeof(Buffer);
}


void *
Bind::Buffer::alloc(bool &heap, size_t len)
{
    if (!heap) {
        if (len <= sizeof(Buffer)) return (void *)this;
        _data = 0;
        _size = 0;
        heap = true;
    }
    
    if (len <= _size) return _data;
    if (_data) std::free(_data);
    if (len <= sizeof(Buffer)) {
        heap = false;
        return (void *)this;
    }
    _data = std::malloc(len);
    if (!_data) throw Exception("Out of memory",
                                CR_OUT_OF_MEMORY,
                                "HY000");
    _size = len;
    return _data;
}


void
Bind::Buffer::free(bool &heap)
{
    if (heap) std::free(_data);
    heap = false;
}




Bind::Bind()
    : _buffer()
    , _length()
    , _type(MYSQL_TYPE_NULL)
    , _null(true)
    , _unsigned(false)
    , _heap(false)
{
}


Bind::~Bind()
{
    _buffer.free(_heap);
}


void
Bind::throw_unsupported_type() const {
    throw Exception("Using unsupported bind type",
                    CR_UNSUPPORTED_PARAM_TYPE,
                    "HY000");
}


Bind::operator MYSQL_BIND()
{
    MYSQL_BIND res = MYSQL_BIND();
    res.is_null = &_null;
    res.length = &_length;
    res.buffer = _buffer.data(_heap);
    res.buffer_length = _buffer.size(_heap);
    res.buffer_type = _type;
    res.is_unsigned = _unsigned;
    return res;
}


Bind &
Bind::init(MYSQL_FIELD *field)
{
    assert(MYSQL_TYPE_NULL == _type); 
    if (!field) return *this;
    
    _unsigned = field->flags & UNSIGNED_FLAG;
    _type = field->type;
    
    switch(field->type) {
    case MYSQL_TYPE_NULL: break;
    // String types
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
    case MYSQL_TYPE_BLOB:
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_BIT:
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_ENUM:
    case MYSQL_TYPE_SET:
    case MYSQL_TYPE_NEWDATE:
    case MYSQL_TYPE_GEOMETRY: _buffer.alloc(_heap, field->max_length); break;
    // Numeric types
    case MYSQL_TYPE_YEAR:
    case MYSQL_TYPE_TINY:     _buffer.alloc(_heap, 1); break;
    case MYSQL_TYPE_SHORT:    _buffer.alloc(_heap, 2); break;
    case MYSQL_TYPE_INT24:
    case MYSQL_TYPE_FLOAT:
    case MYSQL_TYPE_LONG:     _buffer.alloc(_heap, 4); break;
    case MYSQL_TYPE_LONGLONG:
    case MYSQL_TYPE_DOUBLE:   _buffer.alloc(_heap, 8); break;
    // Time types
    case MYSQL_TYPE_TIMESTAMP:
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_TIME:
    case MYSQL_TYPE_DATETIME: _buffer.alloc(_heap, sizeof(MYSQL_TIME)); break;
    default:                  throw_unsupported_type();
    }
    return *this;
}


template<typename T>
bool
Bind::setNumeric(T value, const enum_field_types type, const bool is_uns)
{
    void *const old = _buffer.data(_heap);
    void *data = _buffer.alloc(_heap, sizeof(T));
    *reinterpret_cast<T *>(data) = value;
    _null = false;
    if (type == _type && is_uns == _unsigned && old == data) return false;
    _type = type;
    _unsigned = is_uns;
    return true;
}


bool
Bind::setBuffer(const char *str, size_t len, enum_field_types type)
{
    _null = !str;
    void *const old = _buffer.data(_heap);
    void *data = _buffer.alloc(_heap, len);
    assert(str || !len);
    memcpy(data, str, len);
    _length = len;
    if (type == _type && old == data) return false;
    _type = type;
    return true;
}


bool
Bind::setTime(const Time &time, enum_field_types type)
{
    const MYSQL_TIME &tm = time;
    return setBuffer(reinterpret_cast<const char *>(&tm),
                     sizeof(MYSQL_TIME),
                     type);
}


bool
Bind::setNull()
{
    _null = true;
    return false;
}


bool
Bind::setTinyInt(int8_t value)
{
    return setNumeric(value, MYSQL_TYPE_TINY, false);
}


bool
Bind::setUTinyInt(uint8_t value)
{
    return setNumeric(value, MYSQL_TYPE_TINY, true);
}


bool
Bind::setSmallInt(int16_t value)
{
    return setNumeric(value, MYSQL_TYPE_SHORT, false);
}


bool
Bind::setUSmallInt(uint16_t value)
{
    return setNumeric(value, MYSQL_TYPE_SHORT, true);
}


bool
Bind::setYear(uint16_t value)
{
    // I'm not sure if MYSQL_TYPE_YEAR if allowed as input param
    // return setValue(value), setNumeric(MYSQL_TYPE_YEAR, true);
    return setUSmallInt(value);
}


bool
Bind::setMediumInt(int32_t value)
{
    // I'm not sure if MYSQL_TYPE_INT24 if allowed as input param
    // return setValue(value), setNumeric(MYSQL_TYPE_INT24, false);
    return setInt(value);
}


bool
Bind::setUMediumInt(uint32_t value)
{
    // I'm not sure if MYSQL_TYPE_INT24 if allowed as input param
    // return setValue(value), setNumeric(MYSQL_TYPE_INT24, true);
    return setUInt(value);
}


bool
Bind::setInt(int32_t value)
{
    return setNumeric(value, MYSQL_TYPE_LONG, false);
}


bool
Bind::setUInt(uint32_t value)
{
    return setNumeric(value, MYSQL_TYPE_LONG, true);
}


bool
Bind::setBigInt(int64_t value)
{
    return setNumeric(value, MYSQL_TYPE_LONGLONG, false);
}


bool
Bind::setUBigInt(uint64_t value)
{
    return setNumeric(value, MYSQL_TYPE_LONGLONG, true);
}


bool
Bind::setDouble(double value)
{
    return setNumeric(value, MYSQL_TYPE_DOUBLE, false);
}


bool
Bind::setFloat(float value)
{
    return setNumeric(value, MYSQL_TYPE_FLOAT, false);
}


bool
Bind::setCString(const char *str)
{
    return setBuffer(str, str ? strlen(str) : 0, MYSQL_TYPE_STRING);
}


bool
Bind::setString(const std::string &str)
{
    return setBuffer(str.data(), str.size(), MYSQL_TYPE_STRING);
}


bool
Bind::setBlob(const std::string &str)
{
    return setBuffer(str.data(), str.size(), MYSQL_TYPE_BLOB);
}


bool
Bind::setDateTime(const Time &time)
{
    switch(time.time_type) {
    case MYSQL_TIMESTAMP_DATE:     return setTime(time, MYSQL_TYPE_DATE);
    case MYSQL_TIMESTAMP_TIME:     return setTime(time, MYSQL_TYPE_TIME);
    case MYSQL_TIMESTAMP_DATETIME: return setTime(time, MYSQL_TYPE_DATETIME);
    default: break;
    }
    switch(_type) { // set value without changing type
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_DATETIME:
    case MYSQL_TYPE_TIME:
    case MYSQL_TYPE_TIMESTAMP: return setTime(time, _type);
    default: break;
    }
    return setTime(time, MYSQL_TYPE_DATE);
}


// MYSQL_TYPE_TIMESTAMP is identical to MYSQL_TYPE_DATETIME;
// bool
// Bind::setTimeStamp(const Time &time)
// {
//     return setTime(time, MYSQL_TYPE_TIMESTAMP);
// }


int32_t
Bind::getInt() const
{
    return getBigInt();
}


uint32_t
Bind::getUInt() const
{
    return getUBigInt();
}


std::string
Bind::getString() const
{
    const void *data = _buffer.data(_heap);
    if (_null || !data) return std::string();
    std::ostringstream os;
    switch(_type) {
    case MYSQL_TYPE_NULL:
        return std::string();        
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
    case MYSQL_TYPE_BLOB:
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_BIT:
    case MYSQL_TYPE_ENUM:
    case MYSQL_TYPE_SET:
    case MYSQL_TYPE_NEWDATE:
    case MYSQL_TYPE_GEOMETRY:
        return std::string(reinterpret_cast<const char *>(data), _length);
    case MYSQL_TYPE_TINY:
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_YEAR:
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_INT24:
    case MYSQL_TYPE_LONGLONG:
        if (_unsigned) os << getUBigInt(); else os << getUBigInt();
        break;
    case MYSQL_TYPE_FLOAT:
        os << std::scientific << std::setprecision(9)
           << *reinterpret_cast<const float *>(data);
        break;
    case MYSQL_TYPE_DOUBLE:
        os << std::scientific << std::setprecision(17)
           << *reinterpret_cast<const double *>(data);
        break;
    case MYSQL_TYPE_TIMESTAMP:
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_TIME:
    case MYSQL_TYPE_DATETIME:
        os << Time(*reinterpret_cast<const MYSQL_TIME *>(data));
        break;
    default:
        throw_unsupported_type();
    }
    return os.str();
}


int64_t
Bind::getBigInt() const
{
    if (_unsigned) return getUInt();
    const char *data = reinterpret_cast<char *>(_buffer.data(_heap));
    if (_null || !data) return 0;
    switch(_type) {
    case MYSQL_TYPE_NULL: return 0;
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
    case MYSQL_TYPE_BLOB:
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_BIT:
    case MYSQL_TYPE_ENUM:
    case MYSQL_TYPE_SET:
    case MYSQL_TYPE_NEWDATE:
    case MYSQL_TYPE_GEOMETRY:
        return strtoll(std::string(data, _length).c_str(), NULL, 10);
    case MYSQL_TYPE_TINY:
        return *reinterpret_cast<const int8_t *>(data);
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_YEAR:
        return *reinterpret_cast<const int16_t *>(data);
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_INT24:
        return *reinterpret_cast<const int32_t *>(data);
    case MYSQL_TYPE_LONGLONG:
        return *reinterpret_cast<const int64_t *>(data);
    case MYSQL_TYPE_FLOAT:
        return *reinterpret_cast<const float *>(data);
    case MYSQL_TYPE_DOUBLE:
        return *reinterpret_cast<const double *>(data);
    case MYSQL_TYPE_TIMESTAMP:
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_TIME:
    case MYSQL_TYPE_DATETIME:
        return 0; // TODO: convert DATE/TIME to int
    default:
        throw_unsupported_type();
    }
    return 0;
}


uint64_t
Bind::getUBigInt() const
{
    if (!_unsigned) return getInt();
    const char *data = reinterpret_cast<char *>(_buffer.data(_heap));
    if (_null || !data) return 0;
    switch(_type) {
    case MYSQL_TYPE_NULL: return 0;        
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
    case MYSQL_TYPE_BLOB:
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_BIT:
    case MYSQL_TYPE_ENUM:
    case MYSQL_TYPE_SET:
    case MYSQL_TYPE_NEWDATE:
    case MYSQL_TYPE_GEOMETRY:
        return strtoull(std::string(data, _length).c_str(), NULL, 10);
    case MYSQL_TYPE_TINY:
        return *reinterpret_cast<const uint8_t *>(data);
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_YEAR:
        return *reinterpret_cast<const uint16_t *>(data);
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_INT24:
        return *reinterpret_cast<const uint32_t *>(data);
    case MYSQL_TYPE_LONGLONG:
        return *reinterpret_cast<const uint64_t *>(data);
    case MYSQL_TYPE_FLOAT:
        return *reinterpret_cast<const float *>(data);
    case MYSQL_TYPE_DOUBLE:
        return *reinterpret_cast<const double *>(data);
    case MYSQL_TYPE_TIMESTAMP:
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_TIME:
    case MYSQL_TYPE_DATETIME:
        return 0; // TODO: convert DATE/TIME to int
    default:
        throw_unsupported_type();
    }
    return 0;
}


Time
Bind::getDateTime() const
{
    const char *data = reinterpret_cast<char *>(_buffer.data(_heap));
    if (_null || !data) return Time::none();
    switch(_type) {
    case MYSQL_TYPE_NULL: return Time::none();        
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:
    case MYSQL_TYPE_TINY_BLOB:
    case MYSQL_TYPE_MEDIUM_BLOB:
    case MYSQL_TYPE_LONG_BLOB:
    case MYSQL_TYPE_BLOB:
    case MYSQL_TYPE_VAR_STRING:
    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_VARCHAR:
    case MYSQL_TYPE_BIT:
    case MYSQL_TYPE_ENUM:
    case MYSQL_TYPE_SET:
    case MYSQL_TYPE_NEWDATE:
    case MYSQL_TYPE_GEOMETRY:
        return Time(std::string(data, _length));
    case MYSQL_TYPE_TINY:
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_YEAR:
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_INT24:
    case MYSQL_TYPE_LONGLONG:
    case MYSQL_TYPE_FLOAT:
    case MYSQL_TYPE_DOUBLE:
        break;
    case MYSQL_TYPE_TIMESTAMP:
    case MYSQL_TYPE_DATE:
    case MYSQL_TYPE_TIME:
    case MYSQL_TYPE_DATETIME:
        return Time(*reinterpret_cast<const MYSQL_TIME *>(data));
    default:
        throw_unsupported_type();
    }
    return Time::none();
}


}
