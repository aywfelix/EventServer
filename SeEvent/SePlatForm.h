
#define __WIN32__

#if defined(__WIN32__) || defined(_WIN32) || defined(__WIN64) || defined(__WIN64__) || defined(WIN32) || defined(WIN64)
#define SF_PLATFORM_WIN
#elif defined(linux) || defined(__linux__) || defined(__LINUX) || defined(__LINUX__)
#define SF_PLATFORM_LINUX
#endif

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <winerror.h>
#include <ws2tcpip.h>
#pragma comment(lib,"Ws2_32.lib ")
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in6.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <sys/resource.h>
#endif

#include <cerrno>
#include <climits>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <sys/stat.h>
#include <sys/types.h>


typedef uint8_t		UINT8;
typedef uint16_t	UINT16;
typedef uint32_t	UINT32;
typedef uint32_t	UINT;
typedef uint64_t	UINT64;
typedef int8_t		INT8;
typedef int16_t		INT16;
typedef int32_t		INT32;
typedef int64_t		INT64;
typedef int64_t		Time_t;
typedef uint8_t		BYTE;
typedef unsigned long  ULONG;
typedef unsigned long long ULL64;
typedef long long       LL64;
typedef long        L64;
typedef signed int SINT32;
typedef unsigned int    UINT32;

#define TRUE 1
#define FALSE 0

#define NOMINMAX
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


#ifdef _WIN32
//#define open _open
//#define read _read
//#define close _close
#ifndef fstat
#define fstat _fstati64
#endif
#ifndef stat
#define stat _stati64
#endif
#define mode_t int
#endif

#ifdef _WIN32
#define socket_t intptr_t
#else
#define socket_t int
#define INVALID_SOCKET -1
#endif


#ifdef _WIN32
#define MY_SOCKET_ERROR() WSAGetLastError()
int SocketGetError(socket_t fd);
int gettimeofday(struct timeval* tp, void* tzp);
typedef DWORD		TID;
#else
#define MY_SOCKET_ERROR() (errno)
#define SocketGetError(sock) (errno)
typedef pthread_t	TID;
#endif

#ifdef SF_PLATFORM_WIN
#define SPRINTF sprintf_s
#define SFSTRICMP stricmp
#define SFSLEEP(s) Sleep(s) //millisecond
#define SFGetPID() lexical_cast<std::string>(getpid())

#define		tvsnprintf		_vsnprintf
#define		tsnprintf		_snprintf

#else
#define SPRINTF snprintf
#define SFSTRICMP strcasecmp
#define SFSLEEP(s) usleep(s*1000) //millisecond
#define SFGetPID() lexical_cast<std::string>(getpid())

#define		tvsnprintf		vsnprintf
#define		tsnprintf		snprintf
void SetResource();
#endif

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
/* True iff e is an error that means a read/write operation can be retried. */
#define SOCKET_ERR_RW_RETRIABLE(e)				\
	((e) == EINTR || SOCKET_ERR_IS_EAGAIN(e))
/* True iff e is an error that means an connect can be retried. */
#define SOCKET_ERR_CONNECT_RETRIABLE(e)			\
	((e) == EINTR || (e) == EINPROGRESS)
/* True iff e is an error that means a accept can be retried. */
#define SOCKET_ERR_ACCEPT_RETRIABLE(e)			\
	((e) == EINTR || SOCKET_ERR_IS_EAGAIN(e) || (e) == ECONNABORTED)

/* True iff e is an error that means the connection was refused */
#define SOCKET_ERR_CONNECT_REFUSED(e)					\
	((e) == ECONNREFUSED)

#endif

int SocketCloseOnExec(socket_t fd);

TID CurrentThreadId();

#define LOOP_TIMEOUT 30   // event loop 30ms

