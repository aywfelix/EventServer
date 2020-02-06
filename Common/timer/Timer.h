#pragma once
#include <functional>
#include <atomic>
#include "Timestamp.h"

typedef std::function<void()> TimerCb;

class Timer
{
public:
	Timer(TimerCb& cb, Timestamp& when, int interval);
	~Timer();
	void Run();
	void Reset(Timestamp& now);
	Timestamp Expiration() { return mExpiration; }
	int64_t Seq() { return mSeq; }
	bool IsRepeate() { return mRepeate; }
	
private:
	TimerCb mCb;
	Timestamp mExpiration;
	int mInterval;
	bool mRepeate;
	std::atomic<int64_t> mSeq;
	static std::atomic<int64_t> seq;
};

class TimeId
{
public:
	TimeId() {}
	~TimeId() {}
	TimeId(Timer* pTimer, int64_t seq):mTimer(pTimer), mSeq(seq){}
	Timer* GetTimer() { return mTimer; }
	int64_t GetSeq() { return mSeq; }
private:
	Timer* mTimer{nullptr};
	int64_t mSeq{0};
};
