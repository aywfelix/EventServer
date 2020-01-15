
#define __WIN32__

#if defined(__WIN32__) || defined(_WIN32) || defined(__WIN64) || defined(__WIN64__) || defined(WIN32) || defined(WIN64)
#define SF_PLATFORM_WIN
#elif defined(linux) || defined(__linux__) || defined(__LINUX) || defined(__LINUX__)
#define SF_PLATFORM_LINUX
#endif

#ifdef SF_PLATFORM_WIN
#include <winsock2.h>
#include <windows.h>
#include <winerror.h>
#include <ws2tcpip.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/errno.h>
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

#include "lexical_cast.hpp"

typedef int64_t		Time_t;
typedef uint8_t		BYTE;
typedef unsigned long  ULONG;
typedef unsigned long long ULL64;
typedef long long       LL64;
typedef long        L64;
typedef signed int SINT32;
typedef unsigned int    uint32_t;

#define TRUE 1
#define FALSE 0

#define NOMINMAX
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#ifdef SF_PLATFORM_WIN
int gettimeofday(struct timeval* tp, void* tzp);
typedef DWORD		TID;

#define SPRINTF sprintf_s
#define SFSTRICMP stricmp
#define SFSLEEP(s) Sleep(s) //millisecond
#define SFGetPID() _getpid()

#define		tvsnprintf		_vsnprintf
#define		tsnprintf		_snprintf
#else
typedef pthread_t	TID;
#define SPRINTF snprintf
#define SFSTRICMP strcasecmp
#define SFSLEEP(s) usleep(s*1000) //millisecond
#define SFGetPID() getpid()
#define		tvsnprintf		vsnprintf
#define		tsnprintf		snprintf
void	SetResource();
#endif


TID CurrentThreadId();

#define LOOP_TIMEOUT 30   // event loop 30ms

