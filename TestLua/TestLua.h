#pragma once

#include "sol/sol.hpp"
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

	void RegisterFunc(const std::string& funcname, void(*Func)(ScriptObj& obj));
private:
	sol::state m_state;
};