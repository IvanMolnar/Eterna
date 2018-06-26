// Examples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "Eterna.lib")

#include "..\LuaRegisterClass.h"

class Foo
{
public:

	int add(int x, int y)
	{
		return 42;
	}

	float sub(float x, float y)
	{
		return x - y;
	}

	void registerLua()
	{
		auto t = LuaRegisterClass::getInstance();

		auto luaClass = t->createClass("LuaObjectTest");

		luaClass->template registerLuaFunctions<Foo, int, int, int>("add", this, &Foo::add, 11, 22);
		luaClass->template registerLuaFunctions<Foo, float, float, float>("sub", this, &Foo::sub, 1, 2);

		t->registerClass();
	}
};

int main()
{
	Foo t;
	t.registerLua();
	
	LuaFunction func("add.lua", "testInstance");

	if (ScriptEngine::getInstance()->runFunction(&func))
	{
		int x = 4;
	}

    return 0;
}

