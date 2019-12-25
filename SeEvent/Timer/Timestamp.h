#pragma once
#include "SePlatForm.h"

class Timestamp
{
public:
	Timestamp();
	Timestamp(INT64 millisecond);
	Timestamp(int delay);
	INT64 Now();
	Timestamp Timet(time_t tim);
	Timestamp AddTime(int seconds);
	INT64 MilliSeconds() const { return mmilliseconds; }

	//bool operator<(Timestamp timestamp)
	//{
	//	return mmilliseconds < timestamp.MilliSeconds();
	//}
	//Timestamp operator+(int ti)
	//{
	//	return Timestamp(mmilliseconds + ti);
	//}

	
private:
	INT64 mmilliseconds;
};

bool operator<=(const Timestamp lhs, const Timestamp rhs)
{
	return lhs.MilliSeconds() <= rhs.MilliSeconds();
}

bool operator<(const Timestamp& lhs, const Timestamp& rhs)
{
	return lhs.MilliSeconds() < rhs.MilliSeconds();
}

bool operator==(const Timestamp& lhs, const Timestamp& rhs)
{
	return lhs.MilliSeconds() == rhs.MilliSeconds();
}