#pragma once
#include "SePlatForm.h"
#include "lexical_cast.hpp"
#include <string>

bool IsEqualFloat(const float fval, float epsilon=1e-6);
bool IsEqualDouble(const double dval, double epsilon=1e-15);

template<typename TYPE>
bool StrConvert(const std::string& str, TYPE& value)
{
	try
	{
		value = lexical_cast<TYPE>(str);
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}

std::string TimeToDate(Time_t t);
Time_t DateToTime(const std::string& date);
Time_t GetDigitDate(Time_t t);
INT64 GetTimeS();
INT64 GetTimeMs();

std::string StringFormat(const char* fmt, ...);
void Snprintf(char* buffer, size_t count, const char* format, ...);

#ifndef DELETE_PTR
#define DELETE_PTR(x) if((x)) { delete (x); (x)=nullptr;}
#endif

#ifndef DELETE_PTR_ARR
#define DELETE_PTR_ARR(x) if((x)) { delete[] (x); (x)=nullptr;}
#endif

#if SF_PLATFORM == SF_PLATFORM_WIN
int gettimeofday(struct timeval* tp, void* tzp);
#else
#endif

