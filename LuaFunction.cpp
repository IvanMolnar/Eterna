#include "stdafx.h"
#include "LuaFunction.h"


LuaFunction::LuaFunction(std::string filePath, std::string functionName) :
	_filePath(filePath),
	_functionName(functionName)
{
}

LuaFunction::~LuaFunction()
{
}
