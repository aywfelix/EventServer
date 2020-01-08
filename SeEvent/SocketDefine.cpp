#include "SocketDefine.h"

int GetReadableSizeOnSocket(socket_t fd)
{
#ifdef SF_PLATFORM_WIN
	u_long readn = DEFAULT_SOCKET_READ_LEN;
	if (IoctlSocket(fd, FIONREAD, &readn) < 0)
		return -1;
	return (int)readn;
#else
	int n = DEFAULT_SOCKET_READ_LEN;
	if (IoctlSocket(fd, FIONREAD, &n) < 0)
		return -1;
	return n;
#endif
}