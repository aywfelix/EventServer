#include "SFTest.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <string>
#include "Common/SePlatForm.h"
#include "LogUtil.h"

std::string SFTest::m_content = "";

class Test
{
public:
	void Print(const int32_t& a) { std::cout << a << std::endl; }
	void Print(const int64_t& a) { std::cout << a << std::endl; }
private:
};

void SFTest::TestCpuNum()
{
	GetCpuCores();
}

int main()
{
	//INIT_SFLOG("Test");
	//SFTest::TestRedis();
	//SFTest::TestSplit();
	//SFTest::TestWriteLog();
	//SFTest::TestConsistent();
	//SFTest::TestEncypt();
	//SFTest::TestSnowFlake();
	//SFTest::TestMd5();
	//SFTest::TestFile();
	//SFTest::TestAnyData();
	//SFTest::TestSql();

	//Test test;
	//std::string aa = "aaaaaaaaa";
	//test.Print(100);

	//SFTest::TestCpuNum();

	//SFTest::TestConnMysql();

	//insert into tb(field1,field2) values ("111",22)

	//std::vector<int> num = { 10,20,30,30,20,10,10,20 };
	//num.erase(std::find(num.begin(), num.end(), 20));
	/*auto it = std::remove(num.begin(), num.end(), 20);*/
	//std::for_each(num.begin(), num.end(), [](int& n) {
	//	std::cout << n << " ";
	//	});
	//for (auto it : num)
	//{
	//	std::cout << it << " ";
	//}

	//SFTest::TestExpr();

	//SFTest::TestAStar();
	SFTest::TestFileSystem();

	//Test2* t = new Test2;
	//std::cout << "-------------" << std::endl;
	//std::vector<Test2*> vec;
	//vec.push_back(t);

	while (1)
	{
		sf_sleep(500);
	}
	return 0;
}