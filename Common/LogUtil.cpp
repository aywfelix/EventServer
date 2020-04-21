#include <cstring>
#include <thread>
#include <functional>
#include "Assertx.h"
#include "JsonConfig.h"
#include "LogUtil.h"

std::unique_ptr<LogUtil> g_pLog = nullptr;

bool LogUtil::Init(std::string servername)
{
	m_servername = servername;
	if (m_roll_type == E_ROLL_HOUR)
	{
		m_timeout.SetInterval(3600);
	}
	if (m_roll_type == E_ROLL_DAY)
	{
		m_timeout.SetInterval(3600 * 24);
	}
	if (m_roll_type == E_ROLL_SIZE)
	{
		m_timeout.SetInterval(3 * 60);
	}
	return true;
}

bool LogUtil::CreateLog()
{
	if (!m_timeout.IsTimeOut()) { return false; }
	
	int64_t now = time(0);
	m_logname = m_servername + "_" + std::to_string(now / 60) + ".log"; // 取分钟数日志的名称为：服务器名_分钟数.log
	switch (m_roll_type)
	{
	case E_ROLL_HOUR:
	case E_ROLL_DAY:
		break;
	case E_ROLL_SIZE:
		long size = m_filec.FileSize();
		if (size>0 && size< m_roll_size) { return false; }
		break;
	}
	m_filec.Close();
	m_filec.SetFile(m_LogPath, m_logname);
	if (!m_filec.Open())
	{
		AssertEx(0, "open log file error");
		return false;
	}
	return true;
}

void LogUtil::ThreadLoop()
{
	for (;;)
	{
		if (m_stop) break;
		CreateLog();
		SendLog();
		sf_sleep(50);
	}
}

bool LogUtil::SendLog()
{
	std::string info = "";
	if (m_queue.TryPopQ(info))
	{
#ifdef DEBUG
		std::cout << info << std::endl; 
#else
		m_filec.Write(info.c_str(), sizeof(char), info.length());
#endif
	}
	return true;
}

void LogUtil::Start()
{
	m_stop = false;
	m_thread = std::thread(std::bind(&LogUtil::ThreadLoop, this));
}

void LogUtil::Stop()
{
	m_stop = true;
	m_filec.Close();
	if (m_thread.joinable()) m_thread.join();
}

void LogUtil::Log(int level, const char* file, const char* func, int line, const char* fmt, ...)
{
	if (level < m_level) return;

	UniqueLock lock(m_mutex);
	m_oss.clear();
	m_oss.str("");
	std::string timestr = TimeToDate(time(0));

	char content[256];
	memset(content, '\0', sizeof(content));
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(content, sizeof(content) - 1, fmt, ap);
	va_end(ap);

#ifdef DEBUG
	if (level == E_LOG_FATAL || level == E_LOG_ERR)
	{
		m_oss << "\x1b[31m" << timestr << "|" << file << ":" << line << "|" << func << "|" << content << "\x1b[0m";  //red
	}
	else if (level == E_LOG_WARN)
	{
		m_oss << "\x1b[33m" << timestr << "|" << file << ":" << line << "|" << func << "|" << content << "\x1b[0m"; //yellow
	}
	else if (level == E_LOG_INFO)
	{
		m_oss << "\x1b[32m" << timestr << "|" << file << ":" << line << "|" << func << "|" << content << "\x1b[0m"; //green
	}
	else if (level == E_LOG_DEBUG)
	{
		m_oss << "\x1b[37m" << timestr << "|" << file << ":" << line << "|" << func << "|" << content << "\x1b[0m"; //white
	}
#else
	m_oss << timestr << "|" << file << ":" << line << "|" << func << "|" << content;
#endif
	m_oss << "\n";
	m_queue.PutQ(m_oss.str());
}

bool LogUtil::LoadLogCfg(std::string& logcfg)
{
	return LoadLogCfg(logcfg.c_str());
}

bool LogUtil::LoadLogCfg(const char* logcfg)
{
	JsonConfig json_config;
	if (!json_config.Load(logcfg))
	{
		AssertEx(0, "log path cfg error");
		return false;
	}
	m_level = json_config.m_Root["Log"]["level"].asInt();
	m_roll_type = json_config.m_Root["Log"]["roll"].asInt();
	m_LogPath = json_config.m_Root["Log"]["path"].asString();
	m_roll_size = json_config.m_Root["Log"]["roll_size"].asInt64();
	return true;
}

LogStream& LogUtil::Stream(int level, const char* file, const char* func, int line)
{
	if (m_level < g_pLog->GetLevel()) { return m_logstream; }

	m_logstream.Init(level, file, func, line);
	return m_logstream;
}

void LogStream::Init(int level, const char* file, const char* func, int line)
{
	m_oss.clear();
	m_oss.str("");
	m_level = level;
	m_file = file;
	m_func = func;
	m_line = line;
	// time
	std::string timestr = TimeToDate(time(0));

#ifdef DEBUG
	if (level == E_LOG_FATAL || level == E_LOG_ERR)
	{
		m_oss << "\x1b[31m" << timestr << "|" << file << ":" << line << "|" << func << "|";
	}
	else if (level == E_LOG_WARN)
	{
		m_oss << "\x1b[33m" << timestr << "|" << file << ":" << line << "|" << func << "|";
	}
	else if (level == E_LOG_INFO)
	{
		m_oss << "\x1b[32m" << timestr << "|" << file << ":" << line << "|" << func << "|";
	}
	else if (level == E_LOG_DEBUG)
	{
		m_oss << "\x1b[37m" << timestr << "|" << file << ":" << line << "|" << func << "|";
	}
#else
	m_oss << timestr << "|" << file << ":" << line << "|" << func << ">>>";
#endif
}

void LogStream::Clear()
{
#ifdef DEBUG
	if (m_level == E_LOG_FATAL || m_level == E_LOG_ERR)
	{
		m_oss << "\x1b[0m";  // red
}
	else if (m_level == E_LOG_WARN)
	{
		m_oss << "\x1b[0m"; // yellow
	}
	else if (m_level == E_LOG_INFO)
	{
		m_oss << "\x1b[0m"; // green
	}
	else if (m_level == E_LOG_DEBUG)
	{
		m_oss << "\x1b[0m"; // white
	}
#endif // DEBUG
	m_oss << "\n";
	g_pLog->GetQueue().PutQ(m_oss.str());
}

void LogUtil::LogGame(int type, const char* fmt, ...)
{
	m_oss.clear();
	m_oss.str("");
	std::string timestr = TimeToDate(time(0));

	char content[256];
	memset(content, '\0', sizeof(content));
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(content, sizeof(content) - 1, fmt, ap);
	va_end(ap);

	m_oss << timestr << "|" << content << "\n";
	// 处理业务日志信息
	std::cout << m_oss.str() << std::endl;
}