#pragma once

template<typename T>
class SingleTon
{
public:
	template<typename... Args>
	static T* Instance(Args&&... args)
	{
		if (m_instance == nullptr)
			m_instance = new T(std::forward<Args>(args)...);
		return m_instance;
	}
	static T* GetInstance()
	{
		if (m_instance == nullptr)
			throw std::logic_error("instance is nullptr");
		return m_instance;
	}
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

	virtual void Init(){}

	SingleTon(void) = default;
	virtual ~SingleTon(void) = default;
	SingleTon(const SingleTon&) = delete;
	SingleTon& operator=(const SingleTon&) = delete;
private:
	static T* m_instance;
};