
#include "SFTest.h"
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <iostream>
#include "StringUtil.h"

void Split(const std::string& str, const std::string& delim, std::vector<std::string>& res)
{
	char* p = strtok(const_cast<char*>(str.c_str()), delim.c_str());
	while (p)
	{
		std::string s = p;
		res.emplace_back(s);
		p = strtok(nullptr, delim.c_str());
	}
}

void SFTest::TestSplit()
{
	std::vector<std::string> res;
	std::string str = "13|4343,rdgaerg|sge,sefw,arg|se";
	Split(str, "|,", res);
	std::for_each(res.begin(), res.end(), [](std::string& it) {
		std::cout << it<<std::endl;
		});

	res.clear();
	StringUtil::Split(str, "|,", &res);
	std::for_each(res.begin(), res.end(), [](std::string& it) {
		std::cout << it << std::endl;
		});
}