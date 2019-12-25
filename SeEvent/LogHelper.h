#pragma once

#include <string>
#include <memory>
#include <cstdio>
#include <cstdarg>
#include <thread>

#include "ThreadBase.h"
#include "ConcurrentQueue.hpp"

enum eLogLevel
{
	E_LOG_DEBUG = 1,
	E_LOG_INFO = 2,
	E_LOG_WARN = 3,
	E_LOG_ERR = 4,
	E_LOG_FATAL = 5,
};

class LogHelper {
public:
	LogHelper();
	~LogHelper();
	bool Init(bool termout = true);
	void Log(const char* file, const char* func, int line, int level, const char* fmt, ...);
	void Start();
	void Stop();
	bool LoadInfoFromCfg(std::string& logcfg);
	bool LoadInfoFromCfg(const char* logcfg);
private:
	void ThreadLoop();
	bool SendLog();
	void SetLogLevel(int level);
private:
	ConcurrentQueue<std::string> m_queue;
	int m_level{ 1 };
	bool m_TermColor{ false };
	bool m_TermOut{ false };

	std::thread m_thread;
};

extern std::unique_ptr<LogHelper> g_pLog;

#define LOG_DEBUG(...)   g_pLog->Log(__FILE__, __FUNCTION__, __LINE__, E_LOG_DEBUG, __VA_ARGS__)
#define LOG_INFO(...)    g_pLog->Log(__FILE__, __FUNCTION__, __LINE__, E_LOG_INFO, __VA_ARGS__)
#define LOG_WARN(...)    g_pLog->Log(__FILE__, __FUNCTION__, __LINE__, E_LOG_WARN, __VA_ARGS__)
#define LOG_ERR(...)     g_pLog->Log(__FILE__, __FUNCTION__, __LINE__, E_LOG_ERR, __VA_ARGS__)
#define LOG_FATAL(...)   g_pLog->Log(__FILE__, __FUNCTION__, __LINE__, E_LOG_FATAL, __VA_ARGS__)

#define INIT_SFLOG(a) do{\
g_pLog = std::make_unique<LogHelper>();\
g_pLog->Init(a);\
g_pLog->Start();\
}while (0);