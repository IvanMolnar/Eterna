// Examples.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma comment(lib, "Eterna.lib")
#include "../ScriptEngine.h"

#include <functional>

class foo
{
public:

	int add(int x, int y)
	{
		return data + x;
	}

	int add2(int x)
	{
		return x * x;
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

int CallSomeFuncOnT(lua_State* L)
{
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


	// get args
	// args are provided in reverse order
	
	int i = lua_tointeger(ScriptEngine::getInstance()->_luaState, lua_gettop(ScriptEngine::getInstance()->_luaState));
	//lua_pop(ScriptEngine::getInstance()->_luaState, 1);

	//if (lua_isinteger(ScriptEngine::getInstance()->_luaState, lua_gettop(ScriptEngine::getInstance()->_luaState) -1))
//	{

		int x = lua_tointeger(ScriptEngine::getInstance()->_luaState, lua_gettop(ScriptEngine::getInstance()->_luaState) -1);
//		lua_pop(ScriptEngine::getInstance()->_luaState, 1);
//	}

		lua_remove(ScriptEngine::getInstance()->_luaState, lua_gettop(ScriptEngine::getInstance()->_luaState) - 1);
		lua_remove(ScriptEngine::getInstance()->_luaState, lua_gettop(ScriptEngine::getInstance()->_luaState) - 2);
		i = lua_tointeger(ScriptEngine::getInstance()->_luaState, lua_gettop(ScriptEngine::getInstance()->_luaState));

	// push result of ref.add()
	lua_pushinteger(ScriptEngine::getInstance()->_luaState, ref.add(i, x));

	// return number of results
	return 1;
}

///////////////////////////////////////////////////////////////////

template<class F>
struct function_traits;

// function pointer
template<class R, class... Args>
struct function_traits<R(*)(Args...)> : public function_traits<R(Args...)>
{};

template<class R, class... Args>
struct function_traits<R(Args...)>
{
	using return_type = R;

	static constexpr std::size_t arity = sizeof...(Args);

	template <std::size_t N>
	struct argument
	{
		static_assert(N < arity, "error: invalid parameter index.");
		using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
	};
};

#include <vector>
#include <memory>

std::vector<std::any> paramStack;

template <typename... Args>
void callFunc(Args... args)
{

}

void callFunc(int x, int y)
{
	int z = x + y;
}

template <typename T, typename D>//std::size_t index>
void getValue()
{
//	std::function<int(lua_State*, int, int*)> fun;

//	std::any s;
//	std::any r = lua_tointeger(ScriptEngine::getInstance()->_luaState, lua_gettop(ScriptEngine::getInstance()->_luaState));
	//if (std::is_same<T::argument<index>::type, int>::value)
	/*
	for (size_t i = 0; i < T::arity; i++)
	{
		if (std::is_same<T::argument<i>::type, int>::value)
		{
			//fun = lua_tointegerx;//ScriptEngine::getInstance()->_luaState, -1);
			std::any r = lua_tointeger(ScriptEngine::getInstance()->_luaState, lua_gettop(ScriptEngine::getInstance()->_luaState));
			paramStack.push_back(r);
		}
	}*/

	
	if (std::is_same<D::type, int>::value)
	{
		//fun = lua_tointegerx;//ScriptEngine::getInstance()->_luaState, -1);
		std::any r = lua_tointeger(ScriptEngine::getInstance()->_luaState, lua_gettop(ScriptEngine::getInstance()->_luaState));
		paramStack.push_back(r);
	}


	

//	result.setType<T::argument<index>>();

//	return result;
}


template <typename T>
int fff(void* F)
{
	using Traits = function_traits<T>;

	std::size_t numOfParams = Traits::arity;
	/*
	getValue<Traits, 0>();
	getValue<Traits, 1>();*/

	auto func = [](lua_State *L)
	{ 

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

		std::size_t funCount = Traits::arity;


		getValue<Traits, Traits::argument<0>>();
		getValue<Traits, Traits::argument<1>>();

	
//		getValue<Traits>();

		
	//	int funCount = paramStack.size();

		if (funCount == 1)
		{
			int x = std::any_cast<int>(paramStack[0]);
			F(x);
		}
		else if (funCount == 2)
		{

			int y = std::any_cast<int>(paramStack[0]);
			int x = std::any_cast<int>(paramStack[1]);

			F(x, y);
		}

		return 1;
	};


	lua_pushcfunction(ScriptEngine::getInstance()->_luaState, func);
    lua_setglobal(ScriptEngine::getInstance()->_luaState, "CallSomeFuncOnT");


	return 0;
}



/*
template <typename T>
int function(lua_state* L, T* t)
{
	if (lua_istable(L, 1))
	{ // If we're passed a table, get CData
		lua_getfield(L, 1, "CData");
		lua_replace(L, 1);
	}

	if (!lua_touserdata(L, 1))
	{
		lua_error(L); // longjmp out.
	}

	T& ref = *(T*)lua_touserdata(L, 1);


	if (std::is_same<T, int>::value)
	{

	}

}*/


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
	lua_pushcfunction(ScriptEngine::getInstance()->_luaState, CreateInstanceOfT);
	lua_setglobal(ScriptEngine::getInstance()->_luaState, "CreateInstanceOfT");

	a.data = 111;
//

	void* s = (void*)(a.add);

	fff<decltype(&a.add)>(s);
//	fff<decltype(&a.add2), a.add2>();

	
//	lua_pushcfunction(ScriptEngine::getInstance()->_luaState, CallSomeFuncOnT);
//	lua_setglobal(ScriptEngine::getInstance()->_luaState, "CallSomeFuncOnT");


	LuaFunction func("add.lua", "testInstance");

	if (ScriptEngine::getInstance()->runFunction(&func))
	{
		int x = 4;
	}


    return 0;
}

