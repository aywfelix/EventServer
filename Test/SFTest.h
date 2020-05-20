#pragma once

#include <iostream>
#include "SePlatForm.h"

class SFTest
{
public:
	static void TestRedis();
	static void TestSplit();
	static void TestWriteLog();
	static void TestConsistent();
	static void TestEncypt();
	static void TestSnowFlake();
	static void TestMd5();
	static void TestFile();
	static void TestAnyData();
	static void TestSql();
	static void TestCpuNum();
	static void TestConnMysql();
	static void TestExpr();
	static void TestAStar();
	static void TestFileSystem();
	static void TestInnerCls();
	static void TestTrait();
};

class Test2
{
public:
	Test2()
	{
		std::cout << "init test" << endl;
	}

	Test2(const Test2& t)
	{
		std::cout << "init test1" << endl;
	}
	Test2(const Test2&& t)
	{
		std::cout << "init test2" << endl;
	}

	bool operator==(const Test2& t)
	{
		std::cout << "init test3" << endl;
	}
};
