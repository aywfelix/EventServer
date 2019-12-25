#pragma once

#include <functional>
#include <vector>
#include "Timestamp.h"
#include "Timer.h"
#include "TimerQueue.h"
#include "SePlatForm.h"
#include "Lock.hpp"

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
	void Cancel(TimeId& timeid);

private:
	TimerQueue* mTimeQ;
	Mutex mMutex;
};
