#include "SFTest.h"

#include <vector>
#include <algorithm>
#include <iostream>

int main()
{
	//SFTest::TestRedis();
	//SFTest::TestSplit();
	//SFTest::TestWriteLog();
	//SFTest::TestConsistent();
	SFTest::TestEncypt();
	SFTest::TestSnowFlake();

	std::vector<int> num = { 10,20,30,30,20,10,10,20 };
	num.erase(std::find(num.begin(), num.end(), 20));
	/*auto it = std::remove(num.begin(), num.end(), 20);*/
	//std::for_each(num.begin(), num.end(), [](int& n) {
	//	std::cout << n << " ";
	//	});
	//for (auto it : num)
	//{
	//	std::cout << it << " ";
	//}
	return 0;
}