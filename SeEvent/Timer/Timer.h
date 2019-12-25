#pragma once
#include <functional>
#include <atomic>
#include "Timestamp.h"

typedef std::function<void()> TimerCb;

class Timer
{
public:
	Timer(TimerCb& cb, Timestamp& when, int interval):mCb(cb), mExpiration(when), mInterval(interval)
	{
		mRepeate = interval > 0;
		mSeq.fetch_add(1);
	}
	void Run()
	{
		mCb();
	}
	void Reset(Timestamp& now)
	{
		if (mRepeate)
		{
			mExpiration = now.AddTime(mInterval);
		}
	}
	Timestamp Expiration() { return mExpiration; }
	int64_t Seq() { return mSeq; }
	bool IsRepeate() { return mRepeate; }
private:
	TimerCb mCb;
	Timestamp mExpiration;
	int mInterval;
	bool mRepeate;
	std::atomic<int64_t> mSeq;
};

class TimeId
{
public:
	TimeId():mSeq(0),mTimer(nullptr)
	{}
	TimeId(Timer* pTimer, int64_t seq):mTimer(pTimer), mSeq(seq)
	{}
	Timer* GetTimer() { return mTimer; }
	int64_t GetSeq() { return mSeq; }
private:
	Timer* mTimer;
	int64_t mSeq;
};
