#pragma once
#include "SePlatForm.h"

class Timestamp
{
public:
	Timestamp();
	Timestamp(int64_t millisecond);
	Timestamp(int delay);
	Timestamp Now();
	Timestamp AddTime(int seconds);
	int64_t MilliSeconds() const { return mmilliseconds; }
	bool operator<=(Timestamp& timestamp) const
	{
		if (this == &timestamp)
		{
			return false;
		}
		return mmilliseconds <= timestamp.MilliSeconds();
	}

	bool operator<(const Timestamp& timestamp) const
	{
		if (this == &timestamp)
		{
			return false;
		}
		return mmilliseconds < timestamp.MilliSeconds();
	}

	bool operator==(const Timestamp& timestamp) const
	{
		if (this == &timestamp)
		{
			return false;
		}
		return mmilliseconds == timestamp.MilliSeconds();
	}
	Timestamp operator+(int delay)
	{
		return Timestamp(mmilliseconds + delay);
	}
	
private:
	int64_t mmilliseconds;
};
