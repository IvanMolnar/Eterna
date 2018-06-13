#include "stdafx.h"

#include "ScriptEngine.h"

#include <string>

ScriptEngine::ScriptEngine()
{
	init();
}

ScriptEngine::~ScriptEngine()
{
	lua_close(_luaState);
}

void ScriptEngine::init()
{
	// Create new Lua state and load the lua libraries
	_luaState = luaL_newstate();
	luaL_openlibs(_luaState);
}

bool ScriptEngine::runFunction(LuaFunction* function)
{
	if (luaL_dofile(_luaState, function->_filePath.c_str()))
	{
		// error parsing script file
		std::string errorMessage = lua_tostring(_luaState, -1);
		lua_pop(_luaState, 1);
		return false;
	}

	lua_getglobal(_luaState, function->_functionName.c_str());

	for (auto& arg : function->_args)
	{
		std::string typeName = arg.type().name();

		if (typeName.compare(0, 3, "int") == 0)
		{
			lua_pushinteger(_luaState, std::any_cast<int>(arg));
		}
		else if (typeName.compare(0, 12, "char const *") == 0)
		{
			lua_pushstring(_luaState, std::any_cast<const char *>(arg));
		}
		else if (typeName.compare(0, 23, "class std::basic_string") == 0)
		{
			lua_pushstring(_luaState, std::any_cast<std::string>(arg).c_str());
		}
		else if (typeName.compare(0, 4, "bool") == 0)
		{
			lua_pushboolean(_luaState, std::any_cast<bool>(arg));
		}
		else if (typeName.compare(0, 6, "double") == 0)
		{
			lua_pushnumber(_luaState, std::any_cast<double>(arg));
		}
		else if (typeName.compare(0, 5,"float") == 0)
		{
			lua_pushnumber(_luaState, std::any_cast<float>(arg));
		}
		else
		{
			// unsupported arg
			throw std::invalid_argument("unsupported argument type: " + typeName + " file: " + function->_filePath + " function " + function->_functionName);
		}
	}

	if (lua_pcall(_luaState, function->_args.size(), 1, 0) != 0)
	{
		// error calling function
		std::string errorMessage = lua_tostring(_luaState, -1);
		lua_pop(_luaState, 1);
		return false;
	}

	return true;
}


