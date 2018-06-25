#pragma once

#include "../ResisterCallback.h"


class LuaObjectTest
{
public:
	LuaObjectTest();
	~LuaObjectTest();

	int add(std::int64_t x, std::int64_t y);
	float sub(float x);

	void registerLuaFunctions();



	
	LuaClass* _luaClass;

	static std::vector<std::string> _tableNames;
	static std::vector<LuaClass*> _luaClasses;
};

