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

private:
	
	template <unsigned int index>
	void registerHelper()
	{
		_luaClassCallbacks[index] = getClassInstance<LuaRegisterClass, index>;
	}

	template <unsigned int... index>
	void generateCallbackFunctions(std::index_sequence<index...>)
	{
		int dummy[] = { 0, ((void)(registerHelper<index>()), 0)... };
	}

public:

	LuaRegisterClass() : _classRegistrator(nullptr)
	{
		generateCallbackFunctions(std::make_index_sequence<EternaLimits::maxRegisteredClasses>{});
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

		auto classCallbackFunction = _luaClassCallbacks[registeredClassCount];

		_classRegistrator = new LuaClassRegistrator(className, classCallbackFunction);

		return _classRegistrator;
	}

	void registerClass()
	{
		if (_classRegistrator)
		{
			std::string tableName = _classRegistrator->_luaClass->registerFunctions();

			_tableNames.push_back(tableName);
			_luaClasses.push_back(std::move(_classRegistrator->_luaClass));

			delete _classRegistrator;
		}
	}

	LuaClassRegistrator* _classRegistrator;

	static std::vector<std::string> _tableNames;
	static std::vector<std::unique_ptr<LuaClass>> _luaClasses;

	lua_CFunction _luaClassCallbacks[EternaLimits::maxRegisteredClasses];
};

std::vector<std::string> LuaRegisterClass::_tableNames;
std::vector<std::unique_ptr<LuaClass>> LuaRegisterClass::_luaClasses;