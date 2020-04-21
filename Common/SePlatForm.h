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
#include <sys/time.h>
#include <sys/sysinfo.h>
#endif

#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <ctime>
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

#define sf_snprintf sprintf_s
#define sf_stricmp stricmp
#define sf_sleep(s) Sleep(s) //millisecond
#define sf_getpid() _getpid()
#define sf_sscanf	sscanf_s
#define		tvsnprintf		_vsnprintf
#define		tsnprintf		_snprintf
#else
typedef pthread_t	TID;
#define sf_snprintf snprintf
#define sf_stricmp strcasecmp
#define sf_sleep(s) usleep(s*1000) //millisecond
#define sf_getpid() getpid()
#define		tvsnprintf		vsnprintf
#define		tsnprintf		snprintf
#define sf_sscanf	sscanf
void	SetResource();
#endif

#define TABLE_PATH "../DataTable/"
#define SERVER_CFG "../Config/ServerConf.json"
#define GAME_MAX_PLAYERS 5000
#define LOOP_TIMEOUT 30   // event loop 30ms


TID CurrentThreadId();
void LocalTime(const time_t* timep, struct tm* result);
int GetCpuCores();