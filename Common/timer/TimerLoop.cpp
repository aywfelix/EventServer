#include "TimerLoop.h"
#include "Assertx.h"
#include "TimerQueue.h"

TimerLoop::TimerLoop()
{
	mTimeQ = new TimerQueue;
	Assert(mTimeQ != nullptr);
}

TimerLoop::~TimerLoop()
{
	if (mTimeQ)
	{
		delete mTimeQ;
		mTimeQ = nullptr;
	}
}

void TimerLoop::TimeLoop()
{
	GuardLock lock(mMutex);
	mTimeQ->TimeLoop();
}

TimeId TimerLoop::RunAfter(int delay, TimerCb& cb)
{
	GuardLock lock(mMutex);
	Timestamp timestamp(delay);
	return mTimeQ->AddTimer(new Timer(cb, timestamp, 0));
}

TimeId TimerLoop::RunAt(Timestamp& timestamp, TimerCb& cb)
{
	GuardLock lock(mMutex);
	return mTimeQ->AddTimer(new Timer(cb, timestamp, 0));
}

TimeId TimerLoop::RunAt(time_t& ti, TimerCb& cb)
{
	Timestamp timestamp(ti*1000);
	return RunAt(timestamp, cb);
}

TimeId TimerLoop::RunEvery(int interval, TimerCb& cb)
{
	GuardLock lock(mMutex);
	Timestamp timestamp(interval);  // 从下个间隔时间开始执行
	return mTimeQ->AddTimer(new Timer(cb, timestamp, interval));
}

TimeId TimerLoop::RunAfter(int delay, TimerCb&& cb)
{
	GuardLock lock(mMutex);
	Timestamp timestamp(delay);
	return mTimeQ->AddTimer(new Timer(cb, timestamp, 0));
}

TimeId TimerLoop::RunAt(Timestamp& timestamp, TimerCb&& cb)
{
	GuardLock lock(mMutex);
	return mTimeQ->AddTimer(new Timer(cb, timestamp, 0));
}

TimeId TimerLoop::RunAt(time_t& ti, TimerCb&& cb)
{
	Timestamp timestamp(ti * 1000);
	return RunAt(timestamp, cb);
}

TimeId TimerLoop::RunEvery(int interval, TimerCb&& cb)
{
	GuardLock lock(mMutex);
	Timestamp timestamp(interval);  // 从下个间隔时间开始执行
	return mTimeQ->AddTimer(new Timer(cb, timestamp, interval));
}


void TimerLoop::Cancel(TimeId& timeid)
{
	GuardLock lock(mMutex);
	mTimeQ->CancelTimer(timeid);
}

TimeId TimerLoop::NearestTimer()
{
	return mTimeQ->NearestTimer();
}