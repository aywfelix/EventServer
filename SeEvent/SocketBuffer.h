#pragma once

#include "Assertx.h"

#define MIN_BUFFER_SIZE 128  // min 128 byte
#define MAX_BUFFER_SIZE 10*1024*1024 // 1M byte

struct BufferChain
{
	int read_pos;
	int write_pos;
	int buffer_len;
	char* buffer;
	struct BufferChain* prev;
	struct BufferChain* next;
	int DataLen() { return (write_pos - read_pos); }
	int Left() { return buffer_len - write_pos; }
	bool IsEmpty() { return write_pos == 0 || write_pos==read_pos; }
};

struct BufferChainMgr
{
	struct BufferChain* first;
	struct BufferChain* last;
	struct BufferChain* last_datap; // 指向最后保存数据的缓存链
	int total_len;
	int chain_num{ 0 };
};

class SocketBuffer
{
public:
	void Init();
	void Write(const char* data, int size);
	void Read(char* buf, int size);
	void Clear();
	BufferChain* NewChain(int size);
	char* PullUp();  // 将链中数据放到第一个链中并返回头指针

	char* GetRecvBuf(int size)
	{
		BufferChain* chain = GetWriteChain(size);
		if (chain)
		{
			return chain->buffer + chain->write_pos;
		}
		return nullptr;
	}

	void PostRecvData(int size)
	{
		m_oBuffer.total_len += size;
		m_oBuffer.last->write_pos += size;
	}

	char* GetSendBuf(int& size)
	{
		if (m_oBuffer.last_datap)
		{
			size = m_oBuffer.last_datap->DataLen();
			return m_oBuffer.last_datap->buffer + m_oBuffer.last_datap->read_pos;
		}
		return nullptr;
	}

	void PostSendData(int size)
	{
		m_oBuffer.total_len -= size;
		m_oBuffer.last_datap->read_pos += size;
		if (m_oBuffer.last_datap->IsEmpty())
		{
			m_oBuffer.last_datap = m_oBuffer.last_datap->next;
		}
	}

	int TotalLen();
	BufferChainMgr& GetChainMgr()
	{
		return m_oBuffer;
	}
private:
	BufferChain* GetWriteChain(int size);
	int GetAllocSize(int size);
	void InsertNewChain(BufferChain* chain);
	void AjustChain(BufferChain* chain);
private:
	BufferChainMgr m_oBuffer;
};

