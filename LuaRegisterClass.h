#pragma once

#include "LuaClassRegistrator.h"


template <typename C, int index>
int getClassInstance(lua_State* L)
{
	lua_pushlightuserdata(L, (C::_luaClasses[index]).get());

	luaL_getmetatable(L, C::_tableNames[index].c_str());

	lua_setmetatable(L, -2);

	return 1;
}


class LuaRegisterClass
{

public:

	LuaRegisterClass() : _classRegistrator(nullptr)
	{
		luaCallback[0] = getClassInstance<LuaRegisterClass, 0>;
		luaCallback[1] = getClassInstance<LuaRegisterClass, 1>;
		luaCallback[2] = getClassInstance<LuaRegisterClass, 2>;
		luaCallback[3] = getClassInstance<LuaRegisterClass, 3>;
		luaCallback[4] = getClassInstance<LuaRegisterClass, 4>;
	}

	static std::shared_ptr<LuaRegisterClass> getInstance()
	{
		static auto instance = std::make_shared<LuaRegisterClass>(LuaRegisterClass());
		return instance;
	}

	LuaClassRegistrator* createClass(const std::string& className)
	{
		if (_classRegistrator)
		{
			delete _classRegistrator;
		}

		std::size_t registeredClassCount = _luaClasses.size();

		auto classCallbackFunction = luaCallback[registeredClassCount];

		_classRegistrator = new LuaClassRegistrator(className, classCallbackFunction);

		return _classRegistrator;
	}

	void registerClass()
	{
		if (_classRegistrator)
		{
			_classRegistrator->_luaClass->registerFunctions();

			_tableNames.push_back("luaL_" + _classRegistrator->_className);
			_luaClasses.push_back(std::move(_classRegistrator->_luaClass));

			delete _classRegistrator;
		}
	}

	LuaClassRegistrator* _classRegistrator;

	static std::vector<std::string> _tableNames;
	static std::vector<std::unique_ptr<LuaClass>> _luaClasses;

	// 5 for testing
	lua_CFunction luaCallback[5];
};

std::vector<std::string> LuaRegisterClass::_tableNames;
std::vector<std::unique_ptr<LuaClass>> LuaRegisterClass::_luaClasses;