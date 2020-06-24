#include "SFTest.h"

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <string>
#include "Common/SePlatForm.h"
#include "LogUtil.h"
#include "TestThreadPool.h"

std::string SFTest::m_content = "";

class Test
{
public:
	Test()
	{
		std::cout << "aaaaaaaaaaa" << std::endl;
	}
	Test(const Test& t)
	{
		std::cout << "bbbbbbbbbbb" << std::endl;
	}
	void Print(const int32_t& a) { std::cout << a << std::endl; }
	void Print(const int64_t& a) { std::cout << a << std::endl; }
	int tt;
private:
	std::mutex m_mutex;
	std::mutex m_mutex2;

};

struct MyTest
{
	int tt;
};

template <bool _test, typename ty1, typename ty2>
struct Condition {
	using type = ty1;
};

template <typename ty1, typename ty2>
struct Condition<false, ty1, ty2> {
	using type = ty1;
};

template <bool _test, typename ty1, typename ty2>
using Condition_t = typename Condition<_test, ty1, ty2>::type;


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
	//SFTest::TestFileSystem();
	//SFTest::TestInnerCls();
	//SFTest::TestTrait();
	//SFTest::TestFileSystem();
	//SFTest::TestVisitor();
	//SFTest::TestObjPool();
	//Test2* t = new Test2;
	//std::cout << "-------------" << std::endl;
	//std::vector<Test2*> vec;
	//vec.push_back(t);


	//MyTest myTest;
	//std::vector<MyTest> vec1;
	//std::vector<MyTest> vec2;
	//myTest.tt = 10;
	//vec1.emplace_back(myTest);
	//myTest.tt = 11;
	//vec2.emplace_back(myTest);
	//std::cout <<&vec1[0]<<" "<< vec1[0].tt << std::endl;
	//std::cout << &vec2[0] << " " << vec2[0].tt << std::endl;

	//std::vector<Test> testMap;
	//testMap.resize(5);
	//int i = 0;
	//for (auto it : testMap)
	//{
	//	i++;
	//	it.Print(i);
	//}

	//SFTest::TestGuard();

	//SFTest::_TestThreadPool();
	TestThreadPool m;

	SFTest::TestCommand();

	while (1)
	{
		sf_sleep(500);
	}
	return 0;
}