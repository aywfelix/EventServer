#include "TimerLoop.h"
#include "Assertx.h"

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
	LockGuard lock(mMutex);
	mTimeQ->TimeLoop();
}

TimeId TimerLoop::RunAfter(int delay, TimerCb& cb)
{
	LockGuard lock(mMutex);
	Timestamp timestamp(delay);
	return mTimeQ->AddTimer(new Timer(cb, timestamp, 0));
}

TimeId TimerLoop::RunAt(Timestamp& timestamp, TimerCb& cb)
{
	LockGuard lock(mMutex);
	return mTimeQ->AddTimer(new Timer(cb, timestamp, 0));
}

TimeId TimerLoop::RunAt(time_t& ti, TimerCb& cb)
{
	Timestamp timestamp(ti*1000);
	return RunAt(timestamp, cb);
}

TimeId TimerLoop::RunEvery(int interval, TimerCb& cb)
{
	LockGuard lock(mMutex);
	Timestamp timestamp(interval);  // 从下个间隔时间开始执行
	return mTimeQ->AddTimer(new Timer(cb, timestamp, interval));
}

void TimerLoop::Cancel(TimeId& timeid)
{
	LockGuard lock(mMutex);
	mTimeQ->CancelTimer(timeid);
}