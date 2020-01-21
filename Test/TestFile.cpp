#include "SFTest.h"

#include <cstdio>
#include "StringUtil.h"

std::string TimeToDate(Time_t t)
{
	struct tm ptm;
	LocalTime(&t, &ptm);
	char buf[32];
	memset(buf, '\0', sizeof(buf));
	::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ptm);
	return buf;
}

void SFTest::TestFile()
{
	std::cout << __FILE__ << std::endl;
	//E:\Event_Server\Test\TestFile.cpp
	std::vector<std::string> vec;
	StringUtil::SplitC(__FILE__, "\\", vec);
	std::cout << vec[vec.size() - 1] << std::endl;

	std::cout << TimeToDate(time(0)) << std::endl;
}


