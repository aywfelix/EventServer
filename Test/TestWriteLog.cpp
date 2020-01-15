#include "SFTest.h"

#include "FileHelper.h"
#include "LogHelper.h"
#include <string>

void SFTest::TestWriteLog()
{
	//FileC file;
	//std::string filename = "test.log";
	//std::string filedir = "./";
	//file.SetFile(filedir, filename);
	//file.Open();
	//const char* str = "1234567890";
	//file.Write(str, sizeof(char), 10);
	//file.Close();

	INIT_SFLOG("TestLog");
	CLOG_INFO << "aaaaaaaaaaaaaaaaaaaaa" << CLOG_END;
	CLOG_INFO << "bbbbbbbbbbbbbbbbbb" << CLOG_END;
	for (int i = 0; i < 10; i++)
	{
		CLOG_INFO << "drfgearheh===========" << i << CLOG_END;
		//LOG_INFO("", i);
	}
	g_pLog->Stop();
}