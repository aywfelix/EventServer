#pragma once

#include "SePlatForm.h"

#ifdef SF_PLATFORM_WIN
#include <WS2tcpip.h>

#define socket_t intptr_t
int SocketGetError(socket_t fd);
#define sf_ioctlsocket(s,c,a) ioctlsocket(s,c,a)
#define CONNINPRROGRESS	WSAEWOULDBLOCK
typedef int	socklen_t;
#else
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/in6.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <arpe/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#define socket_t int
typedef sockaddr_in	SOCKADDR_IN;
typedef sockaddr	SOCKADDR;
#define INVALID_SOCKET	-1
#define WSAEWOULDBLOCK	EWOULDBLOCK
#define sf_ioctlsocket(s,c,a) ioctl(s,c,a)
#define CONNINPRROGRESS	EINPROGRESS
#define INVALID_SOCKET -1
#define SocketGetError(sock) (errno)
#endif

#define SOMAXCONN       0x7fffffff
#define DEFAULT_SOCKET_READ_LEN 1024

const int PROTOCOL_TCP = 1;
const int PROTOCOL_UDP = 2;

#define SOCKET_BUFFER_SIZE 50*1024
#define SOCKET_KEEP_ALIVE_INTERVAL 15

#define TIMEOUT 1

// 可以从socket读出的字节数
int GetReadableSizeOnSocket(socket_t fd);

int SocketCloseOnExec(socket_t fd);

// socket error
#ifdef SF_PLATFORM_WIN
#define SOCKET_ERR_IS_EAGAIN(e) \
	((e) == WSAEWOULDBLOCK || (e) == EAGAIN)

#define SOCKET_ERR_RW_RETRIABLE(e)					\
	((e) == WSAEWOULDBLOCK ||					\
	    (e) == WSAEINTR)

#define SOCKET_ERR_CONNECT_RETRIABLE(e)					\
	((e) == WSAEWOULDBLOCK ||					\
	    (e) == WSAEINTR ||						\
	    (e) == WSAEINPROGRESS ||					\
	    (e) == WSAEINVAL)

#define SOCKET_ERR_ACCEPT_RETRIABLE(e)			\
	SOCKET_ERR_RW_RETRIABLE(e)

#define SOCKET_ERR_CONNECT_REFUSED(e)					\
	((e) == WSAECONNREFUSED)
#else
#if EAGAIN == EWOULDBLOCK
#define SOCKET_ERR_IS_EAGAIN(e) \
	((e) == EAGAIN)
#else
#define SOCKET_ERR_IS_EAGAIN(e) \
	((e) == EAGAIN || (e) == EWOULDBLOCK)
#endif
/* True if e is an error that means a read/write operation can be retried. */
#define SOCKET_ERR_RW_RETRIABLE(e)				\
	((e) == EINTR || SOCKET_ERR_IS_EAGAIN(e))
/* True if e is an error that means an connect can be retried. */
#define SOCKET_ERR_CONNECT_RETRIABLE(e)			\
	((e) == EINTR || (e) == EINPROGRESS)
/* True if e is an error that means a accept can be retried. */
#define SOCKET_ERR_ACCEPT_RETRIABLE(e)			\
	((e) == EINTR || SOCKET_ERR_IS_EAGAIN(e) || (e) == ECONNABORTED)

/* True if e is an error that means the connection was refused */
#define SOCKET_ERR_CONNECT_REFUSED(e)					\
	((e) == ECONNREFUSED)

#endif
