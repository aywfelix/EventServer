#include "SF_Test.h"

#include <vector>
#include <algorithm>
#include <iostream>

int main()
{
	//SF_Test::TestRedis();
	//SF_Test::TestSplit();
	//SF_Test::TestWriteLog();
	//SF_Test::TestConsistent();
	SF_Test::TestEncypt();

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