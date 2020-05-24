#include "Visitor.h"
#include <iostream>
#include "SFTest.h"
using namespace std;

class stA;
class stB;

typedef Visitor<stA, stB> myVisitor;

class Base
{
public:
	virtual void Accept(myVisitor&) = 0;
};

class stA : public Base
{
public:
	double val;
	void Accept(myVisitor& v)
	{
		v.Visit(*this);
	}
};

class stB : public Base
{
public:
	int val;
	void Accept(myVisitor& v)
	{
		v.Visit(*this);
	}
};

class PrintVisitor : public myVisitor
{
public:
	void Visit(const stA& t)
	{
		cout << "from stA " << t.val << endl;
	}
	void Visit(const stB& t)
	{
		cout << "from stB " << t.val << endl;
	}
};


void SFTest::TestVisitor()
{
	PrintVisitor vis;
	stA a;
	a.val = 8.88;
	stB b;
	b.val = 9.99;
	Base* base = &a;
	base->Accept(vis);
	base = &b;
	base->Accept(vis);
}