#pragma once

template<typename T>
class SingleTon
{
public:
	static T* Instance()
	{
		static T t;
		return &t;
	}
	virtual bool Init() { return true; }

	SingleTon() = default;
	virtual ~SingleTon() = default;
	SingleTon(const SingleTon&) = delete;
	SingleTon& operator=(const SingleTon&) = delete;
};