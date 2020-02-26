#include "SFTest.h"
#include "SnowFlake.h"

void SFTest::TestSnowFlake()
{
	g_pSnowFlake = std::make_unique<SnowFlake>();
	g_pSnowFlake->Init();
	for (int i = 0; i < 1000; ++i)
	{
		cout << g_pSnowFlake->UniqueId() << endl;
	}

}