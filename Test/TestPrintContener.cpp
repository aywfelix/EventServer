#include "SFTest.h"
#include "prettyprint.hpp"
#include <vector>
#include <map>

void SFTest::TestPrintContener()
{
	std::vector<int> vec;
	vec.push_back(10);
	vec.push_back(5);
	vec.push_back(3);
	vec.push_back(19);
	std::cout << vec << std::endl;
	std::map<int, int> map;
	map.emplace(1, 2);
	map.emplace(2, 3);
	map.emplace(3, 4);
	std::cout << map << std::endl;
}