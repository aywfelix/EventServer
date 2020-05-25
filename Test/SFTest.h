#pragma once

#include <iostream>
#include "SePlatForm.h"


class user
{
public:
	user(){}

	void SetMemId(int32_t id)
	{
		m_id = id;
	}
	int m_id;
	std::string m_str;
};

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
	static void TestObserve();

	static void TestVisitor();
	static void TestObjPool();
	static std::string m_content;
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
