#include "SnowFlake.h"

std::unique_ptr<SnowFlake> g_pSnowFlake = nullptr;

bool SnowFlake::Init()
{
	m_seqMask = ~(-1L << 10);
	m_lasttm = TimeMs();
	m_mechine = m_mechine == 0 ? 1024 : m_mechine;
	return true;
}

uint64_t SnowFlake::UniqueId()
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
		m_seq = 0; // ×î´óÎª1024
	}
	m_lasttm = now;
	int64_t pid = PId();
	uint64_t uid = (now - m_epoch) << 22 | m_mechine << 16 | pid << 10 | m_seq;
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