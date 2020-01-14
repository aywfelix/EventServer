#include "Timestamp.h"

Timestamp::Timestamp()
{
	Now();
}

Timestamp::Timestamp(int64_t milliseconds)
{
	mmilliseconds = milliseconds;
}

Timestamp::Timestamp(int delay)
{
	Now();
	mmilliseconds += delay;
}

Timestamp Timestamp::Now()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	mmilliseconds = (int64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
	return Timestamp(mmilliseconds);
}

Timestamp Timestamp::AddTime(int seconds)
{
	return Timestamp(mmilliseconds + (int64_t)seconds * 1000);
}