#pragma once

extern "C" {
#include "Lua\lua.h"
#include "Lua\lualib.h"
#include "Lua\lauxlib.h"
}

#include "LuaFunction.h"
#include "Config.h"

#include <string>
#include <any>

#include <vector>

namespace EternaModuleInfo
{
	static const char* DllName = "Eterna.dll";
	static const char* GetInstanceFunction = "getInstance";
}

class ScriptEngine
{

public:

	ScriptEngine();
	~ScriptEngine();

	static ScriptEngine* getInstance()
	{
		static ScriptEngine engine;
		return &engine;
	}

	bool runFunction(LuaFunction* function);

	template<typename T>
	T getResult();

	template<>
	std::string getResult()
	{
		std::string result = lua_tostring(_luaState, -1);
		lua_pop(_luaState, 1);

		return result;
	}

	template<>
	int getResult()
	{
		// lua always returns std::int64_t ( long long )
		int result = static_cast<int>(std::any_cast<std::int64_t>(lua_tointeger(_luaState, -1)));
		lua_pop(_luaState, 1);

		return result;
	}

	template<>
	long long getResult()
	{
		long long result = lua_tointeger(_luaState, -1);
		lua_pop(_luaState, 1);

		return result;
	}

	template<>
	bool getResult()
	{
		bool result = lua_toboolean(_luaState, -1);
		lua_pop(_luaState, 1);

		return result;
	}

	template<>
	double getResult()
	{
		double result = lua_tonumber(_luaState, -1);
		lua_pop(_luaState, 1);

		return result;
	}

	template<>
	float getResult()
	{
		float result = std::any_cast<float>(lua_tonumber(_luaState, -1));
		lua_pop(_luaState, 1);

		return result;
	}
	
	std::vector<void*> _instances;

	lua_State* _luaState;
private:
	void init();
};
