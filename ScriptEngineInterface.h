#pragma once

#include "ScriptEngine.h"

namespace EternaModuleInfo
{
	static const char* DllName = "Eterna.dll";
	static const char* GetInstanceFunction = "getInstance";
}


extern "C" __declspec(dllexport)  ScriptEngine* getInstance()
{
	static ScriptEngine engine;
	return &engine;
}