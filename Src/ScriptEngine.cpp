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
		if (arg.type() == typeid(int))
		{
			lua_pushinteger(_luaState, std::any_cast<int>(arg));
		}
		else if (arg.type() == typeid(const char *))
		{
			lua_pushstring(_luaState, std::any_cast<const char *>(arg));
		}
		else if (arg.type() == typeid(std::string))
		{
			lua_pushstring(_luaState, std::any_cast<std::string>(arg).c_str());
		}
		else if (arg.type() == typeid(bool))
		{
			lua_pushboolean(_luaState, std::any_cast<bool>(arg));
		}
		else if (arg.type() == typeid(double))
		{
			lua_pushnumber(_luaState, std::any_cast<double>(arg));
		}
		else if (arg.type() == typeid(float))
		{
			lua_pushnumber(_luaState, std::any_cast<float>(arg));
		}
		else
		{
			// unsupported arg

			std::string argName(arg.type().name());

			throw std::invalid_argument("unsupported argument type: " + argName + " file: " + function->_filePath + " function " + function->_functionName);
			return false;
		}
	}

	if (lua_pcall(_luaState, static_cast<int>(function->_args.size()), 1, 0) != 0)
	{
		// error calling function
		std::string errorMessage = lua_tostring(_luaState, -1);
		lua_pop(_luaState, 1);
		return false;
	}

	return true;
}


