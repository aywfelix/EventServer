#include "Util.h"
#include <cstdlib>
#include <chrono>
#include <cstdarg>
#include <vector>
#include <ctime>

bool IsEqualFloat(const float fval, float epsilon)
{
	return std::abs(fval) <= epsilon;
}

bool IsEqualDouble(const double dval, double epsilon)
{
	return std::abs(dval) <= epsilon;
}

std::string TimeToDate(Time_t t)
{
	struct tm ptm;
	LocalTime(&t, &ptm);
	char buf[32];
	memset(buf, '\0', sizeof(buf));
	strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", &ptm);
	std::string str = buf;
	return str;
}

Time_t GetDigitDate(Time_t t)
{
	struct tm ptm;
	LocalTime(&t, &ptm);
	return (ptm.tm_year + 1900) * 10000 + (ptm.tm_mon + 1) * 100 + ptm.tm_mday;
}

Time_t DateToTime(const std::string& date)
{
	struct tm stm;
	int year, month, day, hour, minute, second;
	sf_sscanf(date.c_str(), "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	stm.tm_year = year - 1900;
	stm.tm_mon = month - 1;
	stm.tm_mday = day;
	stm.tm_hour = hour;
	stm.tm_min = minute;
	stm.tm_sec = second;
	stm.tm_isdst = 0;

	Time_t t = mktime(&stm);
	return t;
}

int64_t GetTimeS()
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int64_t GetTimeMs()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string StringFormat(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	int nsize = std::vsnprintf(nullptr, 0, fmt, ap);
	std::vector<char> buff((nsize + 1), '\0');
	va_start(ap, fmt);
	std::vsnprintf(buff.data(), buff.size(), fmt, ap);
	va_end(ap);
	return buff.data();
}

void Snprintf(char* buffer, int len, const char* format, ...)
{
	va_list argptr;
	va_start(argptr, format);
	int nchars = tvsnprintf((char*)buffer, len, format, argptr);
	va_end(argptr);
	if (nchars == len) { buffer[len - 1] = '\0'; };
}

std::string& StringTrim(std::string& s)
{
	if (s.empty()) return s;
	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}
