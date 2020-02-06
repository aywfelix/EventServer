#include "SFTest.h"

#include "md5.h"
#include <iostream>
using namespace std;

void PrintMD5(const string& str, MD5& md5) {
	cout << "MD5(" << str << ") = " << md5.toString() << endl;
}
void SFTest::TestMd5()
{
	MD5 md5;
	md5.update("");
	PrintMD5("", md5);

	md5.update("a");
	PrintMD5("a", md5);

	md5.update("bc");
	PrintMD5("abc", md5);

	md5.update("defghijklmnopqrstuvwxyz");
	PrintMD5("abcdefghijklmnopqrstuvwxyz", md5);

	md5.reset();
	md5.update("message digest");
	PrintMD5("message digest", md5);

	md5.reset();
	ifstream ifs("testlog");
	if (ifs.good())
	{
		md5.update(ifs);
		PrintMD5("D:\\test.txt", md5);
	}

}