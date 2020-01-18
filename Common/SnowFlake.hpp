#pragma once

#include <cstdint>
#include <chrono>
#include "SePlatForm.h"

class SnowFlake
{
public:
	SnowFlake()
	{
		m_seqMask = ~(-1L << 10);
		m_lasttm = TimeMs();
	}
	virtual ~SnowFlake() {}

	int64_t UniqueId();
	int64_t TimeMs();
	int64_t PId();

	void SetMechine(int64_t mechine)
	{
		m_mechine = mechine;
	}
	int64_t NextMs(int64_t now);

private:
	int64_t m_seq{ 0 };
	int64_t m_mechine;
	int64_t m_lasttm{ -1L };
	int64_t m_seqMask;
	int64_t m_epoch{ 1546272000000L }; //起始时间戳 2019-01-01
};

int64_t SnowFlake::UniqueId()
{
	int64_t now = TimeMs();
	if (now == m_lasttm)
	{
		m_seq = (m_seq + 1) & m_seqMask;
		if (m_seq == 0)
		{
			now = NextMs(now);
		}
	}
	else
	{
		m_seq = 0; //最大为1024
	}
	m_lasttm = now;
	int64_t pid = PId();
	int64_t uid = (now - m_epoch) << 22 | m_mechine << 16 | pid << 10 | m_seq;
	return uid;
}

int64_t SnowFlake::TimeMs()
{
	auto time_now = std::chrono::system_clock::now();
	auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(time_now.time_since_epoch());
	return duration_ms.count();
}
int64_t SnowFlake::PId()
{
	return (int64_t)sf_getpid();
}

int64_t SnowFlake::NextMs(int64_t now)
{
	while (now <= m_lasttm)
	{
		now = TimeMs();
	}
	return now;
}