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
	LogHelper(int level, const char* file, const char* func, int line);
	~LogHelper();

	template<typename T>
	LogHelper& operator<<(const T& log)
	{
		moss << log;
		return *this;
	}

	bool Init(bool termout = true);
	void Log(int level, const char* file, const char* func, int line, const char* fmt, ...);
	
	void Start();
	void Stop();
	bool LoadInfoFromCfg(std::string& logcfg);
	bool LoadInfoFromCfg(const char* logcfg);
	int GetLevel() { return m_level; }
private:
	void ThreadLoop();
	bool SendLog();
	void SetLogLevel(int level);
private:
	ConcurrentQueue<std::string> m_queue;
	int m_level{ 1 };
	int m_line{ 0 };
	const char* m_file;
	const char* m_func;
	bool m_TermOut{ false };
	static std::ostringstream moss;
	std::thread m_thread;
};

extern std::unique_ptr<LogHelper> g_pLog;

#define LOG_DEBUG(...)   g_pLog->Log(E_LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)    g_pLog->Log(E_LOG_INFO, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)    g_pLog->Log(E_LOG_WARN, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_ERR(...)     g_pLog->Log(E_LOG_ERR, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...)   g_pLog->Log(E_LOG_FATAL, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

#define INIT_SFLOG(a) do{\
g_pLog = std::make_unique<LogHelper>();\
g_pLog->Init(a);\
g_pLog->Start();\
}while (0);

// 流风格输出(只用来调试输出)
#define CLOG_DEBUG LogHelper(E_LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__)
#define CLOG_INFO LogHelper(E_LOG_INFO, __FILE__, __FUNCTION__, __LINE__)
#define CLOG_WARN LogHelper(E_LOG_WARN, __FILE__, __FUNCTION__, __LINE__)
#define CLOG_ERR LogHelper(E_LOG_ERR, __FILE__, __FUNCTION__, __LINE__)
#define CLOG_FATAL LogHelper(E_LOG_FATAL, __FILE__, __FUNCTION__, __LINE__)