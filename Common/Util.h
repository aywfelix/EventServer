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
int64_t GetTimeS();
int64_t GetTimeMs();

std::string StringFormat(const char* fmt, ...);
void Snprintf(char* buffer, int len, const char* format, ...);

#ifndef DELETE_PTR
#define DELETE_PTR(x) if((x)) { delete (x); (x)=nullptr;}
#endif

#ifndef DELETE_PTR_ARR
#define DELETE_PTR_ARR(x) if((x)) { delete[] (x); (x)=nullptr;}
#endif

class TimeOut
{
public:
	bool IsTimeOut()
	{
		if (mExpired == 0)
		{
			mExpired = time(0) + mInterval;
			return true;
		}
		int64_t now = time(0);
		if (now > mExpired)
		{
			mExpired += mInterval;
			return true;
		}
		return false;
	}
	int64_t DisTimeOut()
	{
		return mExpired - time(0);
	}
	void SetInterval(int interval)
	{
		mInterval = interval;
	}
private:
	int64_t mExpired{0};
	int mInterval;
};

template<typename T>
class AutoFree
{
public:
	AutoFree(T* t):m_t(t){}
	~AutoFree()
	{
		if (m_t)
		{
			delete m_t;
			m_t = nullptr;
		}
	}
private:
	T* m_t;
};

template <typename T>
T StringToNum(const std::string& str)
{
	std::istringstream iss(str);
	T num;
	iss >> num;
	return num;
}

std::string& StringTrim(std::string& s);