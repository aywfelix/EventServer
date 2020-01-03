
#include <cstring>
#include <sstream>
#include <iostream>
#include "LogHelper.h"
#include "SePlatForm.h"
#include "LibConfig.hpp"

std::unique_ptr<LogHelper> g_pLog = nullptr;

LogHelper::LogHelper() {}
LogHelper::~LogHelper(){}


bool LogHelper::Init(bool termout)
{
	m_TermOut = termout;
	return true;
}

void LogHelper::ThreadLoop()
{
	for (;;)
	{
		SendLog();
		SFSLEEP(10);
	}
}

bool LogHelper::SendLog()
{
	std::string info;
	if (m_queue.TryPopQ(info))
	{
		if (m_TermOut)
		{
			std::cout << info << std::flush;
		}
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
	switch (m_level)
	{
	case E_LOG_FATAL:
	{
		if (level < E_LOG_FATAL)
		{
			return;
		}
	}
	case E_LOG_ERR:
	{
		if (level < E_LOG_ERR)
		{
			return;
		}
	}
	case E_LOG_WARN:
	{
		if (level < E_LOG_WARN)
		{
			return;
		}
	}
	case E_LOG_INFO:
	{
		if (level < E_LOG_INFO)
		{
			return;
		}
	}
	case E_LOG_DEBUG:
	{
		if (level < E_LOG_DEBUG)
		{
			return;
		}
	}
	default:
		break;
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

	if (m_TermOut)
	{
		if (level == E_LOG_FATAL || level == E_LOG_ERR)
		{
			moss << "\x1b[31m" <<time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content << "\x1b[0m";  //red
		}
		else if (level == E_LOG_WARN)
		{
			moss << "\x1b[33m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content << "\x1b[0m"; //yellow
		}
		else if (level == E_LOG_INFO)
		{
			moss << "\x1b[32m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content << "\x1b[0m"; //green
		}
		else if(level == E_LOG_DEBUG)
		{
			moss << "\x1b[37m" << time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content << "\x1b[0m"; //white
		}
	}
	else
	{
		moss << time_stamp << "|" << file << ":" << line << " " << func << " >>>" << content;
	}
	moss << "\n";
	m_queue.PutQ(moss.str());
	moss.clear();
}

bool LogHelper::LoadInfoFromCfg(std::string& logcfg)
{
	if (!LibConfig::Instance().loadcfg(logcfg))
	{
		std::cout << "load log cfg error" << std::endl;
	}
	const Setting& root = LibConfig::Instance().GetRoot();
	const Setting& log = root["log"];
	int level = 1;
	log.lookupValue("default_level", level);
	if (level == 0)
	{
		m_level = 1;
	}
	else
	{
		m_level = level;
	}
	return true;
}

bool LogHelper::LoadInfoFromCfg(const char* logcfg)
{
	std::string strLogcfg = logcfg;
	return LoadInfoFromCfg(strLogcfg);
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