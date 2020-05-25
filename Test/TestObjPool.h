#pragma once

#include <map>
#include <functional>
#include <memory>
#include <cassert>
#include<iostream>


class user
{
public:
	user() {}
	user(const std::string& str):m_str(str){}

	void SetMemId(int id)
	{
		m_id = id;
	}

	int GetMemId()
	{
		return m_id;
	}

	void print(const std::string& s)
	{
		//std::cout << s << std::endl;
	}
	int m_id;
	std::string m_str;
};




int MaxNum = 10;
template <typename T>
class ObjPoolFixed
{
public:
	ObjPoolFixed() {
		m_obj_arr = new std::shared_ptr<T>[MaxNum];
	}
	~ObjPoolFixed() {}

	static ObjPoolFixed<T>* Instance()
	{
		static ObjPoolFixed<T> pool;
		return &pool;
	}

	template<typename... Args>
	void Init(Args&&... args)
	{
		for (int i = 0; i < MaxNum; ++i)
		{
			auto p = std::shared_ptr<T>(new T(std::forward<Args>(args)...), [this](T* p) {
				int i = p->GetMemId();
				m_obj_arr[i] = std::shared_ptr<T>(p);
				printf("================%d", i);
				});
			p->SetMemId(i);
			m_obj_arr[i] = p;
		}
	}
	template<typename... Args>
	std::shared_ptr<T>& Get()
	{
		return m_obj_arr[m_index];
		m_index++;
		//assert(m_index > MaxNum, "out of obj pool range");
	}
private:
	std::shared_ptr<T>* m_obj_arr;
	int m_index;
};


//template<typename T>
//class ObjPool
//{
//	template<typename... Args>
//	using Constructor = std::function<std::shared_ptr<T>(Args...)>;
//public:
//	static ObjPool<T>* Instance()
//	{
//		static ObjPool<T> pool;
//		return &pool;
//	}
//
//	template<typename... Args>
//	void Init(int num, Args&&... args)
//	{
//		if (num <=0 || num > MaxNum)
//		{
//			//throw std::logic_error("out of range");
//			return;
//		}
//		auto constructName = typeid(Constructor<Args...>).name();
//		for (int i=0;i<num;i++)
//		{
//			//m_obj_map.emplace(constructName, std::shared_ptr<T>(new T(std::forward<Args>(args...), [this, constructName](T* p) 
//			//	{
//			//		m_obj_map.emplace(constructName, std::shared_ptr<T>(p));
//			//	})))
//		}
//	}
//
//	template<typename... Args>
//	std::shared_ptr<T> Get()
//	{
//		std::string constructName = typeid(Constructor<Args...>).name();
//		auto range = m_obj_map.equal_range(constructName);
//		for (auto it = range.first;it != range.second; ++it)
//		{
//			auto ptr = it->second;
//			m_obj_map.erase(it);
//			return ptr;
//		}
//		return nullptr;
//	}
//private:
//	std::map<std::string, std::shared_ptr<T>> m_obj_map;
//};