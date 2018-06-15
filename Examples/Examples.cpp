// Examples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "Eterna.lib")
#include "../ScriptEngine.h"


class foo
{
public:

	int add()
	{
		return data;
	}

	int data = 42;
};

foo a;

int CreateInstanceOfT(lua_State* L)
{
	// create new instance
	//new (lua_newuserdata(L, sizeof(foo))) foo();

	// use existing instance
	lua_pushlightuserdata(L, &a);

	return 1;
}

int CallSomeFuncOnT(lua_State* L) {
	if (lua_istable(L, 1))
	{ // If we're passed a table, get CData
		lua_getfield(L, 1, "CData");
		lua_replace(L, 1);
	}

	if (!lua_touserdata(L, 1))
	{
		lua_error(L); // longjmp out.
	}

	foo& ref = *(foo*)lua_touserdata(L, 1);

	// push result of ref.add()
	lua_pushinteger(ScriptEngine::getInstance()->_luaState, ref.add());

	// return number of results
	return 1;
}

int main()
{/*
	LuaFunction func("add.lua", "strTest");
	std::string param = "string test";
	func.setParams(param);

	std::string functionResult;

	if (ScriptEngine::getInstance()->runFunction(&func))
	{
		functionResult = ScriptEngine::getInstance()->getResult<std::string>();
	}
	*/

	a.data = 111;

	lua_pushcfunction(ScriptEngine::getInstance()->_luaState, CreateInstanceOfT);
	lua_setglobal(ScriptEngine::getInstance()->_luaState, "CreateInstanceOfT");
	lua_pushcfunction(ScriptEngine::getInstance()->_luaState, CallSomeFuncOnT);
	lua_setglobal(ScriptEngine::getInstance()->_luaState, "CallSomeFuncOnT");


	LuaFunction func("add.lua", "testInstance");

	if (ScriptEngine::getInstance()->runFunction(&func))
	{
		int x = 4;
	}

    return 0;
}

