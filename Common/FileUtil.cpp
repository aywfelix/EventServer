#include "FileUtil.h"

FileC::~FileC()
{
	Close();
}

bool FileC::Open(const char* mode)
{
	mFp = fopen(mFile, mode);
	if (mFp == nullptr)
	{
		return false;
	}
	return true;
}

bool FileC::Close()
{
	if (mFp)
	{
		fclose(mFp);
		mFp = nullptr;
	}
	return true;
}

bool FileC::Write(const char* ptr, size_t size, size_t count)
{
	if (fwrite((const void*)ptr, size, count, mFp) != count)
	{
		return false;
	}
	return true;
}

bool FileC::Read(void* buf, size_t size, size_t count)
{
	if (fread(buf, size, count, mFp) != count)
	{
		return false;
	}
	return true;
}
// 按照行读取文件
//while (!feof(fp))
//{
//	memset(szTest, 0, sizeof(szTest));
//	fgets(szTest, sizeof(szTest) - 1, fp); // 包含了换行符  
//	printf("%s", szTest);
//}

void FileC::SeekEnd()
{
	fseek(mFp, 0, SEEK_END);
}

long FileC::FileSize()
{
	if (mFp == nullptr) return 0L;
	fseek(mFp, 0L, SEEK_END);
	long size = ftell(mFp);
	fseek(mFp, 0L, SEEK_SET);
	return size;
}

FileCpp::~FileCpp()
{
	Close();
}

bool FileCpp::Open()
{
	mfs.open(mFile, std::ios::in | std::ios::out | std::ios::app);
	if (!mfs.good())
	{
		return false;
	}
	return true;
}

bool FileCpp::Close()
{
	mfs.close();
	return true;
}

bool FileCpp::Write(const char* ptr)
{
	mfs << ptr;
	return true;
}

bool FileCpp::Read(void* buf)
{
	mfs >> buf;
	return true;
}

// 按照行读取
//while(getline(in, line))
//{ }
long FileCpp::FileSize()
{
	mfs.seekg(0, std::ios::end);
	long size = mfs.tellg();
	mfs.seekg(0, std::ios::beg);
	return size;
}

void FileCpp::SeekEnd()
{
	mfs.seekg(0, std::ios::end);
}