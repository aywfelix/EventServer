#pragma once

#include <iostream>
#include <queue>
#include <memory>

static constexpr int32_t poo_step = 10;
template<typename T>
class ObjPool
{
	using shared_obj_t = shared_ptr<T>;
	using pool_t = typename deque<shared_obj_t>;
public:
	static ObjPool<T>* Instance()
	{
		static ObjPool<T> pool;
		return &pool;
	}

	template<typename... Args>
	void Init(Args&&... args)
	{
		m_count += poo_step;
		for (int32_t i = 0; i < poo_step; i++)
		{
			auto pObj = shared_ptr<T>(new T(std::forward<Args>(args)...), [this](T* obj) {
				obj->Clear();
				m_pool.emplace_back(shared_ptr<T>(obj));
				});
			m_pool.emplace_back(pObj);
		}
	}
	template<typename... Args>
	shared_obj_t GetObj(Args&&... args)
	{
		if (m_pool.empty())
		{
			Init(args...);
		}
		auto obj = m_pool.front();
		m_pool.pop_front();
		return obj;
	}

	int32_t EmptySize()
	{
		return m_pool.size();
	}

	int32_t UseSize()
	{
		return (m_count - m_pool.size());
	}
private:
	pool_t m_pool;
	int32_t m_count{ 0 };
};

