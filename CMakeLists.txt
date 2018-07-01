cmake_minimum_required(VERSION 3.12)

project(Eterna
    LANGUAGES CXX
    VERSION 0.1.1
)

#set(CMAKE_HOME_DIRECTORY ${PROJECT_SOURCE_DIR}/Solution)

#message(STATUS, ${CMAKE_HOME_DIRECTORY})

add_library(Eterna_lib
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Eterna.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/Config.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/LuaClassRegistrator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/LuaFunction.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/LuaFunction.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/LuaRegisterClass.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/RegisterCallback.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/ScriptEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/ScriptEngine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Src/ScriptEngineInterface.h
)

set_target_properties( Eterna_lib PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_HOME_DIRECTORY}/Lib/Debug
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_HOME_DIRECTORY}/Lib/Release
)

set_target_properties(Eterna_lib PROPERTIES OUTPUT_NAME "Eterna")

target_compile_features(Eterna_lib PUBLIC cxx_std_17)


set(LUA_ROOT ${CMAKE_BINARY_DIR}/lua)
set(LUA_SOURCE ${LUA_ROOT}/lua)

include(ExternalProject)

ExternalProject_Add(lua_external
                    PREFIX ${LUA_ROOT}
                    GIT_REPOSITORY "https://github.com/lua/lua.git"
                    GIT_TAG "v5-3-4"
                    UPDATE_COMMAND ""
                    PATCH_COMMAND ""
                    INSTALL_COMMAND ""
                    SOURCE_DIR ${LUA_SOURCE}
                    INSTALL_DIR ${LUA_ROOT}/bin
                    SOURCE_SUBDIR ../
                    BINARY_DIR ${LUA_ROOT}
                    GIT_SHALLOW TRUE
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Src/Lua/CMakeLists.txt ${LUA_ROOT}/CMakeLists.txt COPYONLY)


link_directories(${LUA_SOURCE})

target_link_libraries(Eterna_lib lua)

#include_directories(AFTER ${LUA_SOURCE})

add_dependencies(Eterna_lib lua_external)

add_executable(Example
    ${CMAKE_CURRENT_SOURCE_DIR}/Examples/Examples.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Examples/stdafx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Examples/stdafx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Examples/targetver.h
)

target_compile_features(Example PUBLIC cxx_std_17)

#target_link_libraries(Example lua)

#link_directories(${LUA_SOURCE})
#include_directories(AFTER ${LUA_SOURCE})

#link_directories(${CMAKE_HOME_DIRECTORY}/Lib/Debug)
#include_directories(${CMAKE_HOME_DIRECTORY}/Lib/Debug)

target_link_libraries(Example Eterna_lib)
add_dependencies(Example Eterna_lib)
