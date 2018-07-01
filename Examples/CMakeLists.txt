cmake_minimum_required(VERSION 3.12)

add_executable(Example
    ${CMAKE_CURRENT_SOURCE_DIR}/Examples.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/stdafx.h
    ${CMAKE_CURRENT_SOURCE_DIR}/targetver.h
)

target_compile_features(Example PUBLIC cxx_std_17)

set_target_properties(
    Example PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_HOME_DIRECTORY}/Examples")

target_link_libraries(Example debug ${CMAKE_HOME_DIRECTORY}/Lib/Debug/Eterna.Lib)
target_link_libraries(Example debug ${CMAKE_HOME_DIRECTORY}/Lib/Debug/Lua.Lib)

target_link_libraries(Example optimized ${CMAKE_HOME_DIRECTORY}/Lib/Release/Eterna.Lib)
target_link_libraries(Example optimized ${CMAKE_HOME_DIRECTORY}/Lib/Release/Lua.Lib)
