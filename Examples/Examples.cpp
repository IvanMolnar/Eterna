// Examples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "..\Src\LuaRegisterClass.h"

// test class
class Foo
{
public:

	int add(int x, int y)
	{
		return x + y;
	}

	float sub(float x, float y)
	{
		return x - y;
	}
};

int main()
{
	Foo fooInstance;
	
	auto t = LuaRegisterClass::getInstance();

	// expose this class to lua as "LuaObjectTest"
	auto luaClass = t->createClass("LuaObjectTest");

	// register functions with default parameters
	luaClass->template registerLuaFunction<Foo, int, int, int>("add", &fooInstance, &Foo::add, 11, 22);
	luaClass->template registerLuaFunction<Foo, float, float, float>("sub", &fooInstance, &Foo::sub, 1, 2);

	// end of registration for "LuaObjectTest"
	t->registerClass();
	
	// we create instance of LuaFunction
	// it represent Lua function inside .lua file
	LuaFunction luaFunction("Example.lua", "testInstance");

	// we can also call function with parameters
	luaFunction.setParams(42);

	ScriptEngine* scriptEngine = ScriptEngine::getInstance();

	// and run it
	if (ScriptEngine::getInstance()->runFunction(&luaFunction))
	{
		// function executed successfully

		// we can also get result
		int result = scriptEngine->getResult<int>();

		std::cout << "testInstance function returned " << result << std::endl;
	}

	std::cin.get();

    return 0;
}

