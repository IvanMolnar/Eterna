cmake_minimum_required (VERSION 2.6)

if(WIN32)
        add_definitions( -D_CRT_SECURE_NO_WARNINGS )
endif()


add_library (Lua STATIC lua/lapi.c lua/lcode.c lua/lctype.c lua/ldebug.c lua/ldo.c lua/ldump.c lua/lfunc.c lua/lgc.c lua/llex.c lua/lmem.c lua/lobject.c lua/lopcodes.c lua/lparser.c lua/lstate.c lua/lstring.c lua/ltable.c lua/ltm.c lua/lundump.c lua/lvm.c lua/lzio.c lua/lauxlib.c lua/lbaselib.c lua/lbitlib.c lua/lcorolib.c lua/ldblib.c lua/liolib.c lua/lmathlib.c lua/loslib.c lua/lstrlib.c lua/ltablib.c lua/loadlib.c lua/linit.c lua/ltests.c lua/lua.c lua/lutf8lib.c)

set_target_properties( Lua PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_BINARY_DIR}/../bin 
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../bin
)