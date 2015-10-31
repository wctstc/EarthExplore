#pragma once
#include <string>
extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
using namespace std;

#define BEGIN_LUA(L) while(lua_next(L.LuaState, L._Idx)) \
{

#define END_LUA(L) lua_pop(L.LuaState, 1);}

class CLuaObject
{
public:
  CLuaObject(char* name, lua_State* Lua);
  CLuaObject(lua_State* Lua);
  //
  int getInt(const char* Key);
  string getString(const char* Key);
  //
  int getgInt(const char* Key);
  string getgString(const char* Key);
public:
  int _Idx;
  lua_State* LuaState;
};