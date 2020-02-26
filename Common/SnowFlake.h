#pragma once

#include <cstdint>
#include <chrono>
#include <memory>
#include "SePlatForm.h"

class SnowFlake
{
public:
	bool Init();
	uint64_t UniqueId();
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

extern std::unique_ptr<SnowFlake> g_pSnowFlake;