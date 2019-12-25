#include "SePlatForm.h"

int SocketCloseOnExec(socket_t fd)
{
#if !defined(_WIN32)
    int flags;
    if ((flags = fcntl(fd, F_GETFD, NULL)) < 0)
    {
        fprintf(stderr, "fcntl(%d, F_GETFD)", fd);
        return -1;
    }
    if (!(flags & FD_CLOEXEC))
    {
        if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
        {
            fprintf(stderr, "fcntl(%d, F_SETFD)", fd);
            return -1;
        }
    }
#endif
    return 0;
}



#ifdef _WIN32
int SocketGetError(socket_t sock)
{
    int optval, optvallen = sizeof(optval);
    int err = WSAGetLastError();
    if (err == WSAEWOULDBLOCK && sock >= 0)
    {
        if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (char *)&optval,
                       &optvallen))
            return err;
        if (optval)
            return optval;
    }
    return err;
}

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