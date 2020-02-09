#include "SocketBuffer.h"

void SocketBuffer::Init()
{
	m_oBuffer.first = m_oBuffer.last = m_oBuffer.datap = nullptr;
	m_oBuffer.total_len = 0;
	m_oBuffer.chain_num = 0;
}

void SocketBuffer::Write(const char* data, int size)
{
	BufferChain* chain = GetWriteChain(size);
	if (m_oBuffer.datap == nullptr) m_oBuffer.datap = chain;

	memcpy(chain->buffer + chain->write_pos, data, size);
	chain->write_pos += size;
	m_oBuffer.total_len += size;  // data len
}

void SocketBuffer::Read(char* buf, int size)
{
	if (size > m_oBuffer.total_len) size = m_oBuffer.total_len;
	if (size == 0) return;

	int read_len = 0;
	int can_read_len = 0;
	BufferChain* chain = m_oBuffer.datap;
	while (size > 0 && chain)
	{
		can_read_len = chain->write_pos - chain->read_pos;
		if (can_read_len >= size) can_read_len = size;
		memcpy(buf + read_len, chain->buffer + chain->read_pos, can_read_len);
		m_oBuffer.total_len -= can_read_len;
		read_len += can_read_len;
		size -= can_read_len;
		chain->read_pos += can_read_len;
		if (chain->read_pos == chain->write_pos)
		{
			chain->read_pos = chain->write_pos = 0;
			chain = chain->next;
			m_oBuffer.datap = chain;
		}
	}
}

void SocketBuffer::ReadProtoHead(char* buf, int size)
{
	if (m_oBuffer.total_len <= size) return;  // 尽量保证读到完整协议

	int read_len = 0;
	int can_read_len = 0;
	BufferChain* chain = m_oBuffer.datap;
	while (size > 0 && chain)
	{
		can_read_len = chain->write_pos - chain->read_pos;
		if (can_read_len >= size) can_read_len = size;
		memcpy(buf + read_len, chain->buffer + chain->read_pos, can_read_len);
		read_len += can_read_len;
		size -= can_read_len;
		int pos = chain->read_pos + can_read_len;
		if (pos == chain->write_pos) chain = chain->next;
	}
}

void SocketBuffer::Clear()
{
	struct BufferChain* first = m_oBuffer.first;
	while (first != nullptr)
	{
		delete[] first->buffer;
		struct BufferChain* next = first->next;
		delete first;
		first = next;
	}
	m_oBuffer.first = m_oBuffer.last = m_oBuffer.datap = nullptr;
}

BufferChain* SocketBuffer::NewChain(int size)
{
	BufferChain* chain = new BufferChain;
	Assert(chain);
	chain->buffer = new char[size];
	Assert(chain->buffer);
	memset(chain->buffer, 0, size);
	chain->read_pos = chain->write_pos = 0;
	chain->buffer_len = size;
	chain->next = chain->prev = nullptr;
	InsertNewChain(chain);
	return chain;
}

char* SocketBuffer::PullUp()  // 将链中数据放到第一个链中并返回头指针
{
	return nullptr;
}

BufferChain* SocketBuffer::GetWriteChain(int size)
{
	// 写入原则
	// 1、应该从最后一个是否有空闲空间写入数据
	// 2、查找前面已经空闲一个大小最为合适的空链表写入
	// 3、如果空的链表的长度大于（能够写入size的大小）的2倍以上则写入一个新链表
	if (m_oBuffer.last && m_oBuffer.last->Left() >= size)
	{
		return m_oBuffer.last;
	}
	BufferChain* chain = m_oBuffer.first;
	BufferChain* wait = nullptr;
	while (chain && chain->IsEmpty())
	{
		if (chain->buffer_len >= size)
		{
			if (wait == nullptr) wait = chain;
			else if (wait->buffer_len > chain->buffer_len) wait = chain;
		}
		chain = chain->next;
	}
	int alloc_size = GetAllocSize(size);
	if (wait && wait->buffer_len <= (alloc_size << 1))
	{
		if (m_oBuffer.chain_num > 1)
		{// 需要调整链表的位置到最后
			AjustChain(wait);
		}
		return wait;
	}
	return NewChain(alloc_size);
}

int SocketBuffer::TotalLen()
{
	return m_oBuffer.total_len;
}
int SocketBuffer::GetAllocSize(int size)
{
	int alloc_size = 0;
	alloc_size = MIN_BUFFER_SIZE;
	while (alloc_size < size) alloc_size <<= 1;
	Assert(alloc_size < MAX_BUFFER_SIZE);
	return alloc_size;
}

void SocketBuffer::InsertNewChain(BufferChain* chain)
{
	if (m_oBuffer.chain_num == 0)
	{
		m_oBuffer.first = chain;
		m_oBuffer.last = chain;
	}
	else
	{
		m_oBuffer.last->next = chain;
		chain->prev = m_oBuffer.last;
		m_oBuffer.last = chain;
	}
	++m_oBuffer.chain_num;
}

void SocketBuffer::AjustChain(BufferChain* chain)
{
	if (chain->prev)
	{
		chain->prev->next = chain->next;
		chain->next->prev = chain->prev;
	}
	else
	{
		m_oBuffer.first = chain->next;
		chain->next->prev = m_oBuffer.first;
	}
	m_oBuffer.last->next = chain;
	chain->prev = m_oBuffer.last;
	m_oBuffer.last = chain;
	m_oBuffer.last->next = nullptr;
}
