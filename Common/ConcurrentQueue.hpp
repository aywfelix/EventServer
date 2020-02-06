#pragma once

#include "concurrentqueue.h"

// free lock queue
template<typename T>
class ConcurrentQueue : public moodycamel::ConcurrentQueue<T>
{
public:
	ConcurrentQueue(){}
	~ConcurrentQueue(){}
	bool PutQ(const T& object)
	{
		return this->enqueue(object);
	}

	bool PutQBulk(const T& object)
	{
		return this->enqueue(object);
	}

	bool TryPopQ(T& object)
	{
		return this->try_dequeue(object);
	}

	bool TryPopQBulk(T& object)
	{
		return this->try_dequeue(object);
	}
private:
};