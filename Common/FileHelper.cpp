#include "FileHelper.h"

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

void FileC::SeekEnd()
{
	fseek(mFp, 0, SEEK_END);
}

long FileC::FileSize()
{
	fseek(mFp, 0L, SEEK_END);
	long size = ftell(mFp);
	return size;
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

long FileCpp::FileSize()
{
	mfs.seekg(0, std::ios::end);
	long size = mfs.tellg();
	return size;
}

void FileCpp::SeekEnd()
{
	mfs.seekg(0, std::ios::end);
}