#pragma once
#include <set>
#include <map>
#include "Timestamp.h"
#include "Timer.h"

class Entry
{
public:
	Entry(Timestamp timestamp, Timer* pTimer):mTimer(pTimer), mTimestamp(timestamp) {}
	~Entry() {}
	bool operator<(const Entry& entry) const
	{
		return mTimestamp < entry.mTimestamp;
	}
	Timestamp mTimestamp;
	Timer* mTimer{nullptr};
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

