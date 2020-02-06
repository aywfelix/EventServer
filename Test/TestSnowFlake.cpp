#include "SFTest.h"
#include "SnowFlake.hpp"

void SFTest::TestSnowFlake()
{
	SnowFlake snow;
	snow.SetMechine(1024);
	for (int i = 0; i < 1000; ++i)
	{
		cout << snow.UniqueId() << endl;
	}

}