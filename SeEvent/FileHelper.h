#pragma once

#include <cstdio>
#include <fstream>
#include <string>

class File
{
public:
	virtual bool Close() = 0;
	virtual long FileSize() = 0;
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
	bool Open(const char* mode = "a+");
	bool Close();
	bool Write(const void* ptr, size_t size, size_t count);
	bool Read(void* buf, size_t size, size_t count);
	long FileSize();
private:
	FILE* mFp{nullptr};
};

class FileCpp : public File
{
public:
	bool Open();
	bool Close();
	bool Write(const char* ptr);
	bool Read(void * buf);
	long FileSize();
private:
	std::fstream mfs;
};


 