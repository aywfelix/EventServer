
#define SF_PLATFORM_WIN 1
#define SF_PLATFORM_LINUX 2

#define __WIN32__

#if defined(__WIN32__) || defined(_WIN32) || defined(__WIN64) || defined(__WIN64__) || defined(WIN32) || defined(WIN64)
#define SF_PLATFORM SF_PLATFORM_WIN
#elif defined(linux) || defined(__linux__) || defined(__LINUX) || defined(__LINUX__)
#define SF_PLATFORM SF_PLATFORM_LINUX
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
#define open _open
#define read _read
#define close _close
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
#define SE_SOCKET_ERROR() WSAGetLastError()
int SocketGetError(socket_t fd);
int gettimeofday(struct timeval* tp, void* tzp);
#else
#define SE_SOCKET_ERROR() (errno)
#define SocketGetError(socket_t fd) (errno)
#endif

#if SF_PLATFORM == SF_PLATFORM_WIN
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
#endif

int SocketCloseOnExec(socket_t fd);
int CloseSocket(socket_t fd);
