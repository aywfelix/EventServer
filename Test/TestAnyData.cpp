#include <vector>
#include "SFTest.h"
#include "AnyData.hpp"
#include "lexical_cast.hpp"

using std::vector;

void SFTest::TestAnyData()
{
	AnyData any1(100);
	std::string s = "wwww";
	AnyData any2(s);
	AnyData any3("erhyer");
	cout << any1.Get<int>() << endl;

	std::string ss = "100.0f";
	float f = lexical_cast<float>(ss);
	cout << f << endl;
}
