#include "Timer.h"

std::atomic<int64_t> Timer::seq = 0;

Timer::Timer(TimerCb& cb, Timestamp& when, int interval) :mCb(cb), mExpiration(when), mInterval(interval)
{
	mRepeate = interval > 0;
	mSeq = seq.fetch_add(1);
}

Timer::~Timer() {}

void Timer::Run() { mCb(); }

void Timer::Reset(Timestamp& now)
{
	if (mRepeate)
	{
		mExpiration = now.AddTime(mInterval);
	}
}