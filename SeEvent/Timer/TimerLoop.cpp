#include "TimerLoop.h"
#include "Assertx.h"

TimerLoop::TimerLoop()
{
	mTimeQ = new TimerQueue(this);
	Assert(mTimeQ != nullptr);
}

TimerLoop::~TimerLoop()
{

}

bool TimerLoop::IsThisThread()
{
	return mThreadId == CurrentThreadId();
}

void TimerLoop::TimeLoop()
{
	mTimeQ->TimeLoop();
	DoPendingFunctors();
}

void TimerLoop::RunInLoop(const Functor_t& cb)
{
	if (IsThisThread())
	{
		cb();
	}
	else
	{
		PutPendingQ(cb);
	}
}

void TimerLoop::PutPendingQ(const Functor_t& cb)
{
	LockUnique lock(mMutex);
	mPendingFunctors.emplace_back(cb);
}

TimeId TimerLoop::RunAfter(int delay, TimerCb& cb)
{
	Timestamp timestamp(delay);
	return mTimeQ->AddTimer(new Timer(cb, timestamp, 0));
}

TimeId TimerLoop::RunAt(Timestamp& timestamp, TimerCb& cb)
{
	return mTimeQ->AddTimer(new Timer(cb, timestamp, 0));
}

TimeId TimerLoop::RunEvery(int interval, TimerCb& cb)
{
	Timestamp timestamp(interval);  // 从下个间隔时间开始执行
	return mTimeQ->AddTimer(new Timer(cb, timestamp, interval));
}

void TimerLoop::Cancel(TimeId& timeid)
{
	mTimeQ->CancelTimer(timeid);
}

void TimerLoop::DoPendingFunctors()
{
	std::vector<Functor_t> functors;
	LockUnique lock(mMutex);
	functors.swap(mPendingFunctors);
	for (int i=0;i<functors.size();i++)
	{
		functors[i]();
	}
}