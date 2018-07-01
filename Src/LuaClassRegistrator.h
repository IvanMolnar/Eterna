#pragma once

#include "RegisterCallback.h"

class LuaClassRegistrator
{
public:

	LuaClassRegistrator(const std::string& className, lua_CFunction function)
	{
		_className = className;

		_luaClass = std::make_unique<LuaClass>(LuaClass(_className, function));
	}

	~LuaClassRegistrator() {};

	template<typename C, typename R, typename ...Args>
	void registerLuaFunction(const std::string& functionName, C* instance, R(C::*f)(Args... args), Args... defaultValues)
	{
		auto rawFunc = new LuaFunctionT<C, R, Args...>(f, instance, functionName, defaultValues...);
		std::unique_ptr<LuaFunctionInterface> uniquePtr(rawFunc);
		_luaClass->addFunction(std::move(uniquePtr));
	}

	std::unique_ptr<LuaClass> _luaClass;
	std::string _className;
};
