#pragma once

#pragma comment(lib, "Lua\\lua53.lib")

#include "Lua\include\lua.hpp"
#include "LuaFunction.h"

#include <string>
#include <any>

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
		int result = std::any_cast<int>(lua_tointeger(_luaState, -1));
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
	/*
	template<typename T>
	T* registerAndInstantiateClass()
	{
		T **s = (T **)lua_newuserdata(_luaState, sizeof(T *));

		*s = new T();

		luaL_newlibtable(_luaState, (*s)->getRegisterTable());
		luaL_setfuncs(_luaState, (*s)->getRegisterTable(), 0);

		return *s;
	}*/


	


	

	lua_State* _luaState;
private:
	void init();
};