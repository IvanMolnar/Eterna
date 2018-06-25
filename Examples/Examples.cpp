// Examples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "Eterna.lib")

#include "LuaObjectTest.h"


int main()
{
	LuaObjectTest t;
	t.registerLuaFunctions();

	LuaFunction func("add.lua", "testInstance");

	if (ScriptEngine::getInstance()->runFunction(&func))
	{
		int x = 4;
	}

    return 0;
}

