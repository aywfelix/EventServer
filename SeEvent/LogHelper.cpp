
#include <cstring>
#include <sstream>
#include <iostream>
#include "LogHelper.h"
#include "SePlatForm.h"
#include "LibConfig.hpp"
#include "Assertx.h"

std::unique_ptr<LogHelper> g_pLog = nullptr;

LogHelper::LogHelper() {}
LogHelper::~LogHelper(){}


bool LogHelper::Init(std::string servername)
{
	m_ServerName = servername;
	if (m_RollType == E_ROLL_HOUR)
	{
		m_TimeOut.SetInterval(3600);
	}
	if (m_RollType == E_ROLL_DAY)
	{
		m_TimeOut.SetInterval(3600 * 24);
	}
	return true;
}

bool LogHelper::CreateLog()
{
	INT64 now = time(0);
	switch (m_RollType)
	{
	case E_ROLL_HOUR:
	case E_ROLL_DAY:
		if (!m_TimeOut.IsTimeOut())
		{
			return false;
		}
		mFileC.Close();
		m_LogName = m_ServerName + "_" + std::to_string(now / 60) + ".log"; // 取分钟数日志的名称为：服务器名_分钟数.log
		mFileC.SetFile(m_LogPath, m_LogName);
		if (!mFileC.Open())
		{
			AssertEx(0, "open log file error");
			return false;
		}
		break;
	case E_ROLL_SIZE:

		break;
	}
	return true;
}

void LogHelper::ThreadLoop()
{
	for (;;)
	{
		CreateLog();
		SendLog();
		SFSLEEP(10);
	}
}

bool LogHelper::SendLog()
{
	std::string info;
	if (m_queue.TryPopQ(info))
	{
#ifdef DEBUG
		std::cout << info << std::endl; // 默认输出
#endif
		mFileC.Write(info.c_str(), sizeof(char), info.length());
	}
	return true;
}

void LogHelper::Start()
{
	m_thread = std::thread(std::bind(&LogHelper::ThreadLoop, this));
}

void LogHelper::Stop()
{
	if (m_thread.joinable())
	{
		m_thread.join();
	}
}

void LogHelper::SetLogLevel(int level)
{
	m_level = level;
}

void LogHelper::Log(int level, const char* file, const char* func, int line, const char* fmt, ...)
{
	if (level < m_level)
	{
		return;
	}
	// time
	char time_stamp[32];
	memset(time_stamp, '\0', sizeof(time_stamp));
	time_t now = time(0);
	::strftime(time_stamp, sizeof(time_stamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

	char content[256];
	memset(content, '\0', sizeof(content));
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(content, sizeof(content) - 1, fmt, ap);
	va_end(ap);

#ifdef DEBUG
	if (level == E_LOG_FATAL || level == E_LOG_ERR)
	{
		moss << "\x1b[31m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content << "\x1b[0m";  //red
	}
	else if (level == E_LOG_WARN)
	{
		moss << "\x1b[33m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content << "\x1b[0m"; //yellow
	}
	else if (level == E_LOG_INFO)
	{
		moss << "\x1b[32m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content << "\x1b[0m"; //green
	}
	else if (level == E_LOG_DEBUG)
	{
		moss << "\x1b[37m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content << "\x1b[0m"; //white
	}
#else
	moss << time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content;
#endif
	moss << "\n";
	m_queue.PutQ(moss.str());
	moss.clear();
}

bool LogHelper::LoadLogCfg(std::string& logcfg)
{
	if (!LibConfig::Instance().loadcfg(logcfg))
	{
		AssertEx(0, "load log cfg error");
	}
	const Setting& root = LibConfig::Instance().GetRoot();
	const Setting& log = root["log"];
	if (!log.lookupValue("default_level", m_level))
	{
		AssertEx(0, "log level cfg error");
	}
	if (!log.lookupValue("default_roll", m_RollType))
	{
		AssertEx(0, "log roll cfg error");
	}
	if (!log.lookupValue("default_path", m_LogPath))
	{
		AssertEx(0, "log path cfg error");
	}
	return true;
}

bool LogHelper::LoadLogCfg(const char* logcfg)
{
	std::string strLogcfg = logcfg;
	return LoadLogCfg(strLogcfg);
}

LogStream& LogHelper::Stream(int level, const char* file, const char* func, int line)
{
	stream.Init(level, file, func, line);
	return stream;
}

LogStream::LogStream(){}
LogStream::~LogStream(){}

void LogStream::Init(int level, const char* file, const char* func, int line)
{
	m_level = level;
	m_file = file;
	m_func = func;
	m_line = line;
	// time
	char time_stamp[32];
	memset(time_stamp, '\0', sizeof(time_stamp));
	time_t now = time(0);
	::strftime(time_stamp, sizeof(time_stamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
	if (level == E_LOG_FATAL || level == E_LOG_ERR)
	{
		moss << "\x1b[31m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>";
	}
	else if (level == E_LOG_WARN)
	{
		moss << "\x1b[33m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>";
	}
	else if (level == E_LOG_INFO)
	{
		moss << "\x1b[32m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>";
	}
	else if (level == E_LOG_DEBUG)
	{
		moss << "\x1b[37m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>";
	}
}

void LogStream::Clear()
{
	if (m_level < g_pLog->GetLevel())
	{
		moss.clear();
		m_level = 1;
		m_file = nullptr;
		m_func = nullptr;
		m_line = 0;
		return;
	}
	if (m_level == E_LOG_FATAL || m_level == E_LOG_ERR)
	{
		moss << "\x1b[0m";  // red
	}
	else if (m_level == E_LOG_WARN)
	{
		moss << "\x1b[0m"; // yellow
	}
	else if (m_level == E_LOG_INFO)
	{
		moss << "\x1b[0m"; // green
	}
	else if (m_level == E_LOG_DEBUG)
	{
		moss << "\x1b[0m"; // white
	}
	moss << "\n";
	g_pLog->GetQueue().PutQ(moss.str());
	moss.clear();
	m_level = 1;
	m_file = nullptr;
	m_func = nullptr;
	m_line = 0;
}