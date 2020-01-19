#include "SocketDefine.h"

int GetReadableSizeOnSocket(socket_t fd)
{
#ifdef SF_PLATFORM_WIN
	u_long readn = DEFAULT_SOCKET_READ_LEN;
	if (sf_ioctlsocket(fd, FIONREAD, &readn) < 0)
		return -1;
	return (int)readn;
#else
	int n = DEFAULT_SOCKET_READ_LEN;
	if (sf_ioctlsocket(fd, FIONREAD, &n) < 0)
		return -1;
	return n;
#endif
}

#ifdef SF_PLATFORM_WIN
int SocketGetError(socket_t sock)
{
	int optval, optvallen = sizeof(optval);
	int err = WSAGetLastError();
	if (err == WSAEWOULDBLOCK && sock >= 0)
	{
		if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&optval, &optvallen))
			return err;
		if (optval)
			return optval;
	}
	return err;
}
#endif

int SocketCloseOnExec(socket_t fd)
{
#ifdef SF_PLATFORM_LINUX
	int flags;
	if ((flags = fcntl(fd, F_GETFD, NULL)) < 0)
	{
		fprintf(stderr, "fcntl(%ld, F_GETFD)", fd);
		return -1;
	}
	if (!(flags & FD_CLOEXEC))
	{
		if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
		{
			fprintf(stderr, "fcntl(%ld, F_SETFD)", fd);
			return -1;
		}
	}
#endif
	return 0;
}