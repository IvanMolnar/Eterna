#include "stdafx.h"
#include "LuaObjectTest.h"

template <typename C, int index>
int CreateInstanceOfT(lua_State* L)
{
	lua_pushlightuserdata(L, C::_luaClasses[index]);

	luaL_getmetatable(L, C::_tableNames[index].c_str());


	lua_setmetatable(L, -2);

	return 1;
}

std::vector<std::string> LuaObjectTest::_tableNames;
std::vector<LuaClass*> LuaObjectTest::_luaClasses;

LuaObjectTest::LuaObjectTest()
{
}


LuaObjectTest::~LuaObjectTest()
{
}

int LuaObjectTest::add(std::int64_t x, std::int64_t y)
{
	return x + y;
}

float LuaObjectTest::sub(float x)
{
	return 100 - x;
}

void LuaObjectTest::registerLuaFunctions()
{

	std::string className = "LuaObjectTest";
	_luaClass = new LuaClass(className, CreateInstanceOfT<LuaObjectTest, 0>);


	std::string funName = "add";
	auto rawFunc = new LuaFunctionT<LuaObjectTest, int, std::int64_t, std::int64_t>(&LuaObjectTest::add, this, funName, 111, 222);
	std::unique_ptr<LuaFunctionInterface> uniquePtr(rawFunc);
	_luaClass->addFunction(std::move(uniquePtr));

	funName = "sub";
	auto rawFunc2 = new LuaFunctionT<LuaObjectTest, float, float>(&LuaObjectTest::sub, this, funName, 11);
	std::unique_ptr<LuaFunctionInterface> uniquePtr2(rawFunc2);
	_luaClass->addFunction(std::move(uniquePtr2));


	_luaClass->registerFunctions();

	_tableNames.push_back("luaL_" + className);
	_luaClasses.push_back(_luaClass);
}