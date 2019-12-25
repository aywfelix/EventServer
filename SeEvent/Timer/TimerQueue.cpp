#include "TimerQueue.h"
#include "TimerLoop.h"

TimerQueue::TimerQueue(TimerLoop* pLoop)
{
	mLoop = pLoop;
}

TimerQueue::~TimerQueue()
{

}

TimeId TimerQueue::AddTimer(Timer* pTimer)
{
	mLoop->RunInLoop(std::bind(&TimerQueue::CallInsertTimer, this, pTimer)); 
	return TimeId(pTimer, pTimer->Seq());
}

void TimerQueue::CallInsertTimer(Timer* pTimer)
{
	mTimerList.emplace(Entry(pTimer->Expiration(), pTimer));
}

void TimerQueue::TimeLoop()
{
	auto it = mTimerList.begin();
	Timestamp now;
	for (;it != mTimerList.end();)
	{
		if (it->mTimestamp <= now)
		{
			mActiveList.emplace(it->mTimer->Seq(), it->mTimer);
			it = mTimerList.erase(it);
			continue;
		}
		break;
	}
	for (auto it : mActiveList)
	{
		it.second->Run();
		if (it.second->IsRepeate())
		{
			it.second->Reset(now);
			mTimerList.emplace(Entry(it.second->Expiration(), it.second));
		}
	}
	mActiveList.clear();
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