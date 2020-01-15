#pragma once

#include "Timestamp.h"
#include "Timer.h"
#include "Lock.hpp"

class TimerQueue;

class TimerLoop
{
public:
	TimerLoop();
	~TimerLoop();
	void TimeLoop();
	TimeId RunAfter(int delay, TimerCb& cb);
	TimeId RunAt(Timestamp& timestamp, TimerCb& cb);
	TimeId RunAt(time_t& ti, TimerCb& cb);
	TimeId RunEvery(int interval, TimerCb& cb);

	TimeId RunAfter(int delay, TimerCb&& cb);
	TimeId RunAt(Timestamp& timestamp, TimerCb&& cb);
	TimeId RunAt(time_t& ti, TimerCb&& cb);
	TimeId RunEvery(int interval, TimerCb&& cb);

	void Cancel(TimeId& timeid);

	TimeId NearestTimer();
private:
	TimerQueue* mTimeQ;
	Mutex mMutex;
};
