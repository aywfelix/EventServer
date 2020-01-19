
#include "ThreadBase.h"

bool ThreadBase::Start()
{
    Init();   //call thread init
    m_thread = std::thread(std::bind(&ThreadBase::ThreadLoop, this));
	m_bActive = true;
	return true;
}

bool ThreadBase::Stop()
{
    m_bActive = false;
    Join();
	return true;
}

void ThreadBase::Join()
{
    if(m_thread.joinable())
    {
        m_thread.join();
    }
}
