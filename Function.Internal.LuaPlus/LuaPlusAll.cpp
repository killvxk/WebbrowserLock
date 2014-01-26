#define LUA_FORCE_USE_LONGJMP
#ifndef LUAPLUS_ALL
#define LUAPLUS_ALL
#endif
#define loslib_c
#include "LuaLink.h"
LUA_EXTERN_C_BEGIN
#include "lua/lapi.c"
#include "lua/lauxlib.c"
#include "lua/lbaselib.c"
#include "lua/lcode.c"
#include "lua/ldblib.c"
#include "lua/ldebug.c"
#include "lua/ldo.c"
#include "lua/ldump.c"
#include "lua/lfunc.c"
#include "lua/lgc.c"
#include "lua/linit.c"
#include "lua/liolib.c"
#include "lua/llex.c"
#include "lua/lmathlib.c"
#include "lua/lmem.c"
#include "lua/lobject.c"
#include "lua/lopcodes.c"
#include "lua/loslib.c"
#include "lua/lparser.c"
#include "lua/lstate.c"
#include "lua/lstring.c"
#include "lua/lstrlib.c"
#include "lua/ltable.c"
#include "lua/ltablib.c"
#include "lua/ltm.c"
#include "lua/lundump.c"
#include "lua/lvm.c"
#include "lua/lzio.c"
#include "lwstrlib.c"
#include "LuaPlusAddons.c"
LUA_EXTERN_C_END
#include "LuaPlus.cpp"
#include "LuaPlus_Libs.cpp"
#include "LuaPlusFunctions.cpp"
#include "LuaState.cpp"
#include "LuaStateOutFile.cpp"
#include "LuaState_DumpObject.cpp"
#include "LuaObject.cpp"
#include "LuaTableIterator.cpp"
LUA_EXTERN_C_BEGIN
#include "lua/loadlib.c"
#if defined(LUA_WIN)
#include "popen.c"
#endif // LUA_WIN
LUA_EXTERN_C_END
