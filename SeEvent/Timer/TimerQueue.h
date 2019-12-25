#pragma once
#include <set>
#include <map>
#include "Timestamp.h"
#include "Timer.h"

struct Entry
{
	Entry(Timestamp timestamp, Timer* pTimer) :mTimer(pTimer), mTimestamp(timestamp) {}
	Timestamp mTimestamp;
	Timer* mTimer;
	bool operator<(const Entry& entry) const
	{
		return mTimestamp.MilliSeconds() < entry.mTimestamp.MilliSeconds();
	}
};
//using Entry_t = std::pair<Timestamp, Timer*>;
using TimeList_t = std::set<Entry>;
using ActiveTimer_T = std::map<int64_t, Timer*>;

class TimerLoop;

class TimerQueue
{
public:
	TimerQueue(TimerLoop* pLoop);
	~TimerQueue();
	TimeId AddTimer(Timer* pTimer);
	void CallInsertTimer(Timer* pTimer);
	void TimeLoop();
	void CancelTimer(TimeId& timeid);
private:
	TimeList_t mTimerList;
	ActiveTimer_T mActiveList;
	TimerLoop* mLoop;
};

