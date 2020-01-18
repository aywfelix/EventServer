#include "TimerQueue.h"
#include "TimerLoop.h"

TimerQueue::TimerQueue()
{
}

TimerQueue::~TimerQueue()
{
	Clear();
}

TimeId TimerQueue::AddTimer(Timer* pTimer)
{
	mTimerList.emplace(Entry(pTimer->Expiration(), pTimer));
	return TimeId(pTimer, pTimer->Seq());
}

void TimerQueue::TimeLoop()
{
	auto it = mTimerList.begin();
	Timestamp now;
	for (;it != mTimerList.end();)
	{
		if (it->mTimestamp <= now)
		{
			it->mTimer->Run();
			if (it->mTimer->IsRepeate())
			{
				it->mTimer->Reset(now);
			}
			else
			{
				it = mTimerList.erase(it);
			}
			continue;
		}
		break;
	}
}

void TimerQueue::CancelTimer(TimeId& timeid)
{
	for (auto it = mTimerList.begin();it != mTimerList.end();)
	{
		if (it->mTimer->Seq() == timeid.GetSeq())
		{
			it = mTimerList.erase(it);
		}
	}
}

void TimerQueue::Clear()
{
	for (auto it : mTimerList)
	{
		delete (Timer*)it.mTimer;
	}
	mTimerList.clear();
}

TimeId TimerQueue::NearestTimer()
{
	auto first = mTimerList.begin();
	if (first != mTimerList.end())
	{
		return TimeId(first->mTimer, first->mTimer->Seq());
	}
	return TimeId();
}
