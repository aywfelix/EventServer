#pragma once

#include <sol/sol.hpp>
#include <string>

struct ScriptObj
{
public:
	void PrintName() { std::cout << name << std::endl; }
	std::string name{"aaaaaaaaaaaaaaa"};
};

void lua_call_PrintName(ScriptObj& obj)
{
	obj.PrintName();
}

class CppLua
{
public:
	CppLua();
	~CppLua();
	bool LoadScript(const std::string& filename);
	bool CallLuaFunction(const std::string& funcname);
	bool CallLuaFunction(const std::string& funcname, ScriptObj& obj);

	template<typename... Args>
	bool CallLuaFunc(const std::string& funcname, Args&&... args);

	void RegisterFunc(const std::string& funcname, void(*Func)(ScriptObj& obj));
private:
	sol::state m_state;
};

template<typename ...Args>
inline bool CppLua::CallLuaFunc(const std::string& funcname, Args&& ...args)
{
	if (funcname == "" || funcname.length() == 0) return false;
	m_state[funcname](std::forward<Args>(args)...);
	return true;
}
