/****************************************************************************
   Copyright (C) 2013 Monty Program AB
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
   
   You should have received a copy of the GNU Library General Public
   License along with this library; if not see <http://www.gnu.org/licenses>
   or write to the Free Software Foundation, Inc., 
   51 Franklin St., Fifth Floor, Boston, MA 02110, USA

   Part of this code includes code from the PHP project which
   is freely available from http://www.php.net
*****************************************************************************/
#include <mysql.h>
#include <stdio.h>
#include <string.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#ifndef SEC_PART_DIGITS
#define SEC_PART_DIGITS 6
#endif

size_t mariacpp_time_to_string(const MYSQL_TIME *tm, char *time_str, size_t len)
{
  size_t length;

  if (!time_str || !len)
    return 0;

  unsigned int digits= MIN((tm->second_part) ? SEC_PART_DIGITS : 0, 15);

  switch(tm->time_type) {
    case MYSQL_TIMESTAMP_DATE:
      length= snprintf(time_str, len, "%04u-%02u-%02u", tm->year, tm->month, tm->day);
      digits= 0;
      break;
    case MYSQL_TIMESTAMP_DATETIME:
      length= snprintf(time_str, len, "%04u-%02u-%02u %02u:%02u:%02u", 
                      tm->year, tm->month, tm->day, tm->hour, tm->minute, tm->second);
      break;
    case MYSQL_TIMESTAMP_TIME:
      length= snprintf(time_str, len, "%s%02u:%02u:%02u",
                       (tm->neg ? "-" : ""), tm->hour, tm->minute, tm->second);
    break;
    default:
      time_str[0]= '\0';
      return 0;
      break;
  }
  if (digits && (len < length))
  {
    char helper[16];
    snprintf(helper, 16, ".%%0%du", digits);
    length+= snprintf(time_str + length, len - length, helper, digits);
  }
  return length;
}

// Copied from my_stmt_codec.c
my_bool mariacpp_str_to_TIME(const char *str, size_t length, MYSQL_TIME *tm)
{
  my_bool is_time=0, is_date=0, has_time_frac=0;
  char *p= (char *)str;

  if ((p= strchr(str, '-')) && p <= str + length)
    is_date= 1;
  if ((p= strchr(str, ':')) && p <= str + length)
    is_time= 1;
  if ((p= strchr(str, '.')) && p <= str + length)
    has_time_frac= 1;

  p= (char *)str;
 
  memset(tm, 0, sizeof(MYSQL_TIME));

  if (is_date)
  {
    sscanf(str, "%d-%d-%d", &tm->year, &tm->month, &tm->day);
    p= strchr(str, ' ');
    if (!p)
    {
      tm->time_type= MYSQL_TIMESTAMP_DATE;
      return 0;
    }
  }
  if (has_time_frac)
  {
    sscanf(p, "%d:%d:%d.%ld", &tm->hour, &tm->minute, &tm->second, &tm->second_part);
    tm->time_type= (is_date) ? MYSQL_TIMESTAMP_DATETIME : MYSQL_TIMESTAMP_TIME;
    return 0;
  }
  if (is_time)
  {
    sscanf(p, "%d:%d:%d", &tm->hour, &tm->minute, &tm->second);
    tm->time_type= (is_date) ? MYSQL_TIMESTAMP_DATETIME : MYSQL_TIMESTAMP_TIME;
    return 0;
  }
  return 1;
}



  
