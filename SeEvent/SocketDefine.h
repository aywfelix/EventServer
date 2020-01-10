#pragma once

#include "SePlatForm.h"

#ifdef SF_PLATFORM_WIN
#include <WS2tcpip.h>
#define GETERROR	WSAGetLastError()
#define IoctlSocket(s,c,a) ioctlsocket(s,c,a)
#define CONNINPRROGRESS	WSAEWOULDBLOCK
typedef int	socklen_t;
#else
#include <sys/socket.h>
#include <netinet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpe/inet.h>
#include <sys/errno.h>
typedef sockaddr_in	SOCKADDR_IN;
typedef sockaddr	SOCKADDR;
#define INVALID_SOCKET	-1
#define SOCKET_ERROR	-1
#define GETERROR	errno
#define WSAEWOULDBLOCK	EWOULDBLOCK
#define IoctlSocket(s,c,a) ioctl(s,c,a)
#define CONNINPRROGRESS	EINPROGRESS
#define INVALID_SOCKET -1
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
