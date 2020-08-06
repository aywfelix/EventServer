#include "TestLua.h"

CppLua::CppLua()
{
	m_state.open_libraries();
}
CppLua::~CppLua()
{

}

bool CppLua::LoadScript(const std::string& filename)
{
	if (filename == "" || filename.length() == 0) return false;
	m_state.script_file(filename);
	return true;
}

bool CppLua::CallLuaFunction(const std::string& funcname)
{
	if (funcname == "" || funcname.length() == 0) return false;
	m_state[funcname]();
	return true;
}

bool CppLua::CallLuaFunction(const std::string& funcname, ScriptObj& obj)
{
	if (funcname == "" || funcname.length() == 0) return false;
	m_state[funcname](obj);
	return true;
}

void CppLua::RegisterFunc(const std::string& funcname, void(*Func)(ScriptObj& obj))
{
	m_state.set_function(funcname, Func);
}

int main()
{
	CppLua cpp_lua;
	ScriptObj obj;
	cpp_lua.RegisterFunc("lua_call_PrintName", lua_call_PrintName);
	cpp_lua.LoadScript("script.lua");
	cpp_lua.CallLuaFunction("print_v");
	cpp_lua.CallLuaFunction("print_name", obj);
	cpp_lua.CallLuaFunc("print_test", 1, 3, 4, 5);
	return 0;
}