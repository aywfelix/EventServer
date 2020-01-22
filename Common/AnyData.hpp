#pragma once

#include <variant>
#include <cstdint>
#include <string>
#include <cstring>
#include <exception>

class AnyData {
public:
	template<typename T>
	AnyData(T v):m_data(v) {}

	AnyData(const std::string& v) : m_data(v) {}
	
	AnyData(const AnyData& ref) : m_data(ref.m_data) {}

	template<typename T>
	bool operator !=(T v)
	{
		T n = *this;
		return n != v;
	}

	template<typename T>
	AnyData& operator +=(T v)
	{
		T n = *this;
		n += v;
		operator=(n);
		return *this;
	}

	template<typename T>
	AnyData& operator -=(T v)
	{
		T n = *this;
		n -= v;
		operator=(n);
		return *this;
	}

	template<typename T>
	AnyData& operator=(T& v)
	{
		m_data = v;
		return *this;
	}
	template<typename T>
	T Get()
	{
		try
		{
			T v = std::get<T>(m_data);
			return v;
		}
		catch (const std::exception & e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	template<typename T>
	operator T() const
	{
		try
		{
			T v = std::get<T>(m_data);
			return v;
		}
		catch (const std::exception & e)
		{
			std::cerr << e.what() << '\n';
		}
	}
private:
	std::variant< int32_t, int64_t, float, double, bool, std::string> m_data;
};