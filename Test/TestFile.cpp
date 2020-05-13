#include "SFTest.h"

#include <cstdio>
#include "StringUtil.h"
#include <filesystem>

using namespace std;
using namespace std::filesystem;

std::string TimeToDate(Time_t t)
{
	struct tm ptm;
	LocalTime(&t, &ptm);
	char buf[32];
	memset(buf, '\0', sizeof(buf));
	::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &ptm);
	return buf;
}

void SFTest::TestFile()
{
	std::cout << __FILE__ << std::endl;
	//E:\Event_Server\Test\TestFile.cpp
	std::vector<std::string> vec;
	StringUtil::SplitC(__FILE__, "\\", vec);
	std::cout << vec[vec.size() - 1] << std::endl;

	std::cout << TimeToDate(time(0)) << std::endl;
}

void SFTest::TestFileSystem()
{
	string path = "./";
	for (auto it : directory_iterator(path))
	{
		cout << it.path() << endl;
	}
	if (exists(path))
	{
		cout << "path exist" << endl;
	}
	directory_entry entry(path);
	if (entry.status().type() == file_type::directory)
	{
		cout << "path directory" << endl;
	}

	//void copy(const path & from, const path & to) ：目录复制
   //path absolute(const path & pval, const path & base = current_path()) ：获取相对于base的绝对路径
   //bool create_directory(const path & pval) ：当目录不存在时创建目录
   //bool create_directories(const path & pval) ：形如 / a / b / c这样的，如果都不存在，创建目录结构
   //bool exists(const path & pval) ：用于判断path是否存在
   //uintmax_t file_size(const path & pval) ：返回目录的大小
   //file_time_type last_write_time(const path & pval) ：返回目录最后修改日期的file_time_type对象
   //bool remove(const path & pval) ：删除目录
   //uintmax_t remove_all(const path & pval) ：递归删除目录下所有文件，返回被成功删除的文件个数
   //void rename(const path & from, const path & to) ：移动文件或者重命名
   // path filename()   ：返回文件名
   // path stem():返回文件名不带后缀
   // path extension() :返回文件后缀
   // bool is_absolute() :是不是绝对路径
   // bool is_relative() :是否为相对路径
   // path relative_path()
   // path absolute_path()
   // path parent_path()  :父路径
   // path& replace_extension() :替换后缀名
}
