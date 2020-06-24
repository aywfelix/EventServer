#include "TestGuard.h"
#include "SFTest.h"

#include <functional>

using namespace std;

template<class T>
class guard
{
public:
	using param_t = typename std::conditional<std::is_reference<T>::value || std::is_pointer<T>::value, T, T&>::type;
	using func_t = std::function<void(param_t)>;
	guard(param_t param, func_t&& destory) :m_param(param), m_destory(std::move(destory)) {}
	guard(param_t param, func_t&& create, func_t&& destory)
		: m_param(param), m_destory(std::move(destory)) {
		create(m_param);
	}
	~guard(void) { m_destory(m_param); }

	guard(void) = delete;
	guard(const guard&) = delete;
	guard(const guard&&) = delete;
	guard& operator=(const guard&) = delete;
	guard& operator=(const guard&&) = delete;
private:
	param_t m_param;
	func_t m_destory;
};


class Test_Guard
{
public:
	void print()
	{
		std::cout << "xxxxxxxxxxx" << std::endl;
	}
	void print2()
	{
		std::cout << "xxxxxxxxxxx2222222222" << std::endl;
	}
};

void SFTest::TestGuard()
{
	Test_Guard *test = new Test_Guard;
	auto my_func = std::bind(&Test_Guard::print, test);
	guard<Test_Guard*> test_guard(test, my_func, my_func);
	guard<Test_Guard*> test_guard2(test, &Test_Guard::print, &Test_Guard::print2);
}