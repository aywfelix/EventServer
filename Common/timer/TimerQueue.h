#pragma once
#include <set>
#include <map>
#include "Timestamp.h"
#include "Timer.h"

struct Entry
{
	Entry(Timestamp timestamp, Timer* pTimer):mTimer(pTimer), mTimestamp(timestamp) {}
	Timestamp mTimestamp;
	Timer* mTimer;
	bool operator<(const Entry& entry) const
	{
		return mTimestamp < entry.mTimestamp;
	}
};

using TimeList_t = std::set<Entry>;

class TimerQueue
{
public:
	TimerQueue();
	~TimerQueue();
	TimeId AddTimer(Timer* pTimer);
	void TimeLoop();
	void CancelTimer(TimeId& timeid);
	void Clear();
	TimeId NearestTimer();
private:
	TimeList_t mTimerList;
};

