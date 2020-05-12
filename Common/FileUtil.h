#pragma once

#include <cstdio>
#include <fstream>
#include <string>

class File
{
public:
	virtual ~File(){}
	virtual bool Close() = 0;
	virtual long FileSize() = 0;
	virtual void SeekEnd() = 0;
	void SetFile(std::string& path, std::string& file)
	{
		path.append(file);
		mFile = path.c_str();
	}
	
protected:
	const char* mFile{ nullptr };
};
// 简单文件操作相关类
class FileC : public File
{
public:
	~FileC();
	bool Open(const char* mode = "a+"); // 读写方式，没有创建，文件指针指向末尾
	bool Close();
	bool Write(const char* ptr, size_t size, size_t count);
	bool Read(void* buf, size_t size, size_t count);
	void SeekEnd();
	long FileSize();
private:
	FILE* mFp{nullptr};
};

class FileCpp : public File
{
public:
	~FileCpp();
	bool Open();
	bool Close();
	bool Write(const char* ptr);
	bool Read(void * buf);
	void SeekEnd();
	long FileSize();
private:
	std::fstream mfs;
};

// windows
//#include <corecrt_io.h>
//int size = filelength(fileno(fp));
 