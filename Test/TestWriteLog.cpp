#include "SF_Test.h"

#include "FileHelper.h"
#include <string>

void SF_Test::TestWriteLog()
{
	FileC file;
	std::string filename = "test.log";
	std::string filedir = "./";
	file.SetFile(filedir, filename);
	file.Open();
	const char* str = "1234567890";
	file.Write(str, sizeof(char), 10);
	file.Close();
}