#include "Timestamp.h"

Timestamp::Timestamp()
{
	Now();
}

Timestamp::Timestamp(INT64 milliseconds)
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
	mmilliseconds = (INT64)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
	return Timestamp(mmilliseconds);
}

Timestamp Timestamp::AddTime(int seconds)
{
	return Timestamp(mmilliseconds + (INT64)seconds * 1000);
}