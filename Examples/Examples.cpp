// Examples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "Eterna.lib")
#include "../ScriptEngine.h"

int main()
{
	LuaFunction func("add.lua", "strTest");
	std::string param = "string test";
	func.setParams(param);

	std::string functionResult;

	if (ScriptEngine::getInstance()->runFunction(&func))
	{
		functionResult = ScriptEngine::getInstance()->getResult<std::string>();
	}



    return 0;
}

