#pragma once

template<typename... Types>
class Visitor;

template<typename T, typename... Types>
class Visitor<T, Types...> : Visitor<Types...>
{
public:
	using Visitor<Types...>::Visit; // 避免隐藏基类的同名方法
	virtual void Visit(const T&) = 0;
};

template<typename T>
class Visitor<T>
{
public:
	virtual void Visit(const T&) = 0;
};