#pragma once

#include <list>
#include <any>
#include <string>

class LuaFunction
{
public:
	LuaFunction(std::string filePath, std::string functionName);
	~LuaFunction();

	template<typename... Params>
	void setParams(Params... params);

	std::list<std::any> _args;

	std::string _functionName;
	std::string _filePath;
};

template <typename... Params>
void LuaFunction::setParams(Params... params)
{
	_args = { params ... };
}