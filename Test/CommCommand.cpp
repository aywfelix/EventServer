#include "CommCommand.h"
#include "SFTest.h"

int add(int a, int b)
{
	return a + b;
}

void testvoid(int a, int b)
{
	cout << "test void minus" << endl;
}

class FuncTest
{
public:
	int add(int a, int b)
	{
		return a + b;
	}

	int minus(int a, int b) const
	{
		return a - b;
	}
};

void SFTest::TestCommand()
{
	FuncTest functest;
	FuncWraper<int> cmd;
	cmd.Wrap(&FuncTest::add, &functest, 1, 2);
	auto ret = cmd.Excute();
	cout << "ret =="<< ret << endl;

	cmd.Wrap(&FuncTest::minus, &functest, 2, 1);
	ret = cmd.Excute();
	cout << "ret ==" << ret << endl;

	FuncWraper<void> cmd2;
	cmd2.Wrap(testvoid, 1, 2);
	cmd2.Excute();
	
	cmd2.Wrap([](const std::string& str) { cout << str << endl; }, "abc");
	cmd2.Excute();
}