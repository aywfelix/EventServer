#include "SePlatForm.h"



#ifdef SF_PLATFORM_WIN
int gettimeofday(struct timeval* tp, void* tzp)
{
	time_t clock;
	struct tm _tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	_tm.tm_year = wtm.wYear - 1900;
	_tm.tm_mon = wtm.wMonth - 1;
	_tm.tm_mday = wtm.wDay;
	_tm.tm_hour = wtm.wHour;
	_tm.tm_min = wtm.wMinute;
	_tm.tm_sec = wtm.wSecond;
	_tm.tm_isdst = -1;
	clock = mktime(&_tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}
#endif

TID CurrentThreadId()
{
#ifdef SF_PLATFORM_WIN
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

#ifdef SF_PLATFORM_LINUX
void SetResource()
{
	if(getrlimit(RLIMIT_CORE, &rlim)==0) 
	{
		rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_CORE, &rlim_new) != 0) 
		{
			rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
			(void)setrlimit(RLIMIT_CORE, &rlim_new);
		}
	}
}
#endif