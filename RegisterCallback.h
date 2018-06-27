#pragma once

#include <any>
#include <functional>
#include <type_traits>

#include "ScriptEngine.h"

#include <map>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>

template <class ...Args>
struct ArgsHelper
{
	static constexpr std::size_t arity = sizeof...(Args);

	template <std::size_t N>
	struct argument
	{
		using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
	};
};

template <typename C, int index>
int regDummy(lua_State* L)
{
	if (lua_istable(L, 1))
	{
		// If we're passed a table, get CData
		lua_getfield(L, 1, "CData");
		lua_replace(L, 1);
	}

	if (!lua_touserdata(L, 1))
	{
		lua_error(L); // longjmp out.
	}

	C& ref = *(C*)lua_touserdata(L, 1);
	//lua_pop(L, 1);

	int numArgs = lua_gettop(L) - 2;

	for (int i = numArgs; i > -1; i--)
	{
		std::any result;

		int type = lua_type(L, -1);


		if (type == LUA_TSTRING)
		{
			result = lua_tostring(L, -1);
		}
		else if (type == LUA_TBOOLEAN)
		{
			result = lua_toboolean(L, -1);
		}
		else if (type == LUA_TNUMBER)
		{
			double d = lua_tonumber(L, -1);

			if (d == static_cast<int>(d))
			{
				// it's int
				result = lua_tointeger(L, -1);
			}
			else
			{
				// it's double
				result = d;
			}
		}

		lua_pop(L, 1);

		if (result.has_value())
		{
			ref.setFunctionParam(i, result, index);
		}

	}

	auto returnType = ref.getReturnType(index);

	switch (returnType)
	{
	case LuaFunctionInterface::returnType::eInt:
		lua_pushinteger(L, ref.template getFunctionResult<int>(index));
		break;
	case LuaFunctionInterface::returnType::eFloat:
		lua_pushnumber(L, ref.template getFunctionResult<float>(index));
		break;
	case LuaFunctionInterface::returnType::eDouble:
		lua_pushnumber(L, ref.template getFunctionResult<double>(index));
		break;
	case LuaFunctionInterface::returnType::eBool:
		lua_pushboolean(L, ref.template getFunctionResult<bool>(index));
		break;
	case LuaFunctionInterface::returnType::eString:
		lua_pushstring(L, ref.template getFunctionResult<std::string>(index).c_str());
		break;
	}

	return 1;
};

class LuaFunctionInterface
{
public:

	virtual void setParam(unsigned int index, std::any param) = 0;
	virtual std::string& getName() = 0;
	virtual std::any returnResult() = 0;

	enum class returnType
	{
		eInt,
		eDouble,
		eFloat,
		eBool,
		eString
	};

	returnType _returnType;
};

template <typename C, typename R, typename... Args>
class LuaFunctionT : public LuaFunctionInterface
{
	using Params = ArgsHelper<Args...>;
	using originalFunctionSignature = R(C::*)(Args... args);

public:

	LuaFunctionT(R(C::*f)(Args... args), C* p,  std::string name, Args... args) : _orgFunction(f), _name(name)
	{
		if (std::is_same<R, int>::value || std::is_same<R, std::int64_t>::value)
		{
			_returnType = returnType::eInt;
		}
		else if (std::is_same<R, float>::value)
		{
			_returnType = returnType::eFloat;
		}
		else if (std::is_same<R, double>::value)
		{
			_returnType = returnType::eDouble;
		}
		else if (std::is_same<R, bool>::value)
		{
			_returnType = returnType::eBool;
		}
		else if (std::is_same<R, std::string>::value)
		{
			_returnType = returnType::eString;
		}

		_params = { args... };

		updateParams(std::make_index_sequence<ArgsHelper<Args...>::arity>{});
	}

	template <std::size_t... index>
	void updateParams(std::index_sequence<index...>)
	{
		_function = std::bind(_orgFunction, _origClassPointer, std::any_cast<Params::argument<index>::type>(_params[index])...);
	}

	std::any returnResult() override
	{
		return _function();
	}

	void setParam(unsigned int index, std::any param) override
	{

		bool paramsReset = false;

		// workaround for std::int64_t
		// all int values passed from lua are std::int64_t
		if (_params[index].type() == typeid(int) && param.type() == typeid(std::int64_t))
		{
			paramsReset = true;

			_params[index].reset();
			std::int64_t r = std::any_cast<std::int64_t>(param);
			param = static_cast<int>(r);
		}
		// workaround conversion from double to float, etc...
		else if (_params[index].type() == typeid(float))
		{
			paramsReset = true;

			_params[index].reset();

			if (param.type() == typeid(double))
			{
				double r = std::any_cast<double>(param);
				param = static_cast<float>(r);
			}
			else if (param.type() == typeid(std::int64_t))
			{
				std::int64_t r = std::any_cast<std::int64_t>(param);
				param = static_cast<float>(r);
			}
		}

		if (paramsReset == false && _params[index].type() != param.type())
		{
			std::ostringstream ss;
			ss << "unsuported argument on index " << index << " Argument type expected: " << _params[index].type().name() << " but received: " << param.type().name();

			throw std::invalid_argument(ss.str());
		}

		_params[index] = param;

		updateParams(std::make_index_sequence<ArgsHelper<Args...>::arity>{});
	}


	std::string& getName() override
	{
		return _name;
	}

	std::vector<std::any> _params;
	std::function<R(void)> _function;
	originalFunctionSignature _orgFunction;
	C* _origClassPointer;
	std::string _name;
	lua_CFunction _LuaCFunction;
};

class LuaClass
{

private:

	template <unsigned int index>
	void registerHelper()
	{
		sFooRegs[index].func = regDummy<LuaClass, index>;
	}

	template <unsigned int... index>
	void generateCallbackFunctions(std::index_sequence<index...>)
	{
		int dummy[] = { 0, ((void)(registerHelper<index>()), 0)... };
	}

public:

	LuaClass(std::string className, lua_CFunction function) : _functionIndex(1), _className(className)
	{
		generateCallbackFunctions(std::make_index_sequence<EternaLimits::maxRegisteredClasses>{});

		sFooRegs[0].func = function;
		sFooRegs[0].name = "get";
	}
	
	void addFunction(std::unique_ptr<LuaFunctionInterface> function)
	{
		_functionMap[_functionIndex] = std::move(function);
		++_functionIndex;
	}

	void setFunctionParam(unsigned int index, std::any param, unsigned int functionIndex)
	{
		_functionMap[functionIndex]->setParam(index, param);
	}

	template <typename T>
	T getFunctionResult(unsigned int functionIndex)
	{
		T result;

		std::any r = _functionMap[functionIndex]->returnResult();

		result = std::any_cast<T>(r);

		return result;
	}

	LuaFunctionInterface::returnType getReturnType(unsigned int functionIndex)
	{
		return _functionMap[functionIndex]->_returnType;
	}

	std::string registerFunctions()
	{
		unsigned int index = 1;
		for (auto& it : _functionMap)
		{
			sFooRegs[index].name = it.second->getName().c_str();
			++index;
		}

		sFooRegs[index].func = NULL;
		sFooRegs[index].name = NULL;

		std::string tableName = "luaL_" + _className;

		auto L = ScriptEngine::getInstance()->_luaState;

		luaL_newmetatable(L, tableName.c_str());

		// Register the C functions _into_ the metatable we just created.
		luaL_setfuncs(L, sFooRegs, 0);

		lua_pushvalue(L, -1);

		lua_setfield(L, -1, "__index");

		lua_setglobal(L, _className.c_str());

		return tableName;
	}

	unsigned int _functionIndex;

	std::map<unsigned int, std::unique_ptr<LuaFunctionInterface>> _functionMap;
	luaL_Reg sFooRegs[EternaLimits::maxRegisteredClasses];
	std::string _className;
};
