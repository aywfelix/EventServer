#include "SFTest.h"
#include <iostream>
using namespace std;

class MyClass
{
public:
	struct Inner
	{
		int a;
	};

	void GetA()
	{
		Inner inner;
		inner.a = 1;
	}
private:

};



void SFTest::TestInnerCls()
{
	MyClass cls;
	cls.GetA();
}
