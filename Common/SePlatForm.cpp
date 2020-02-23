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

void LocalTime(const time_t* timep, struct tm* result)
{
#ifdef SF_PLATFORM_WIN
	localtime_s(result, timep);
#else
	localtime_r(timep, result);
#endif
}

#ifdef SF_PLATFORM_LINUX
void SetResource()
{
	// set dump core size
	struct rlimit rlim, rlim_new;
	if (getrlimit(RLIMIT_CORE, &rlim) == 0)
	{
		rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
		if (setrlimit(RLIMIT_CORE, &rlim_new) != 0)
		{
			rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
			(void)setrlimit(RLIMIT_CORE, &rlim_new);
		}
	}
	// set can open max file num
	if (getrlimit(RLIMIT_NOFILE, &rlim) == 0)
	{
		rlim_new.rlim_cur = rlim_new.rlim_max = 8192;
		if (setrlimit(RLIMIT_NOFILE, &rlim_new) != 0)
		{
			rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
			(void)setrlimit(RLIMIT_NOFILE, &rlim_new);
		}
	}
}
#endif

int GetCpuCores()
{
	int cpu_cores = 0;
#ifdef SF_PLATFORM_WIN
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	cpu_cores = sysInfo.dwNumberOfProcessors;
#else
	cores = sysconf(_SC_NPROCS_CONF);
#endif
	return cpu_cores+1;
}