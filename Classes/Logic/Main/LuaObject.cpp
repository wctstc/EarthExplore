#include "LuaObject.h"

CLuaObject::CLuaObject( char* name, lua_State* Lua )
{
  LuaState = Lua;
  lua_getglobal(LuaState, name);
  _Idx = lua_gettop(LuaState);
  lua_pushnil(LuaState);
}

CLuaObject::CLuaObject(lua_State* Lua)
{
  //lua_getglobal(LuaState, name);
  LuaState = Lua;
}

int CLuaObject::getInt( const char* Key )
{
  lua_pushstring(LuaState, Key);
  lua_gettable(LuaState, -2);
  int ret = (int)lua_tonumber(LuaState, -1);
  lua_pop(LuaState, 1);
  return ret;
}

string CLuaObject::getString( const char* Key )
{
  lua_pushstring(LuaState, Key);
  lua_gettable(LuaState, -2);
  string ret = (string)lua_tostring(LuaState, -1);
  lua_pop(LuaState, 1);
  return ret;
}

int CLuaObject::getgInt( const char* Key )
{
  lua_getglobal(LuaState, Key);
  return (int)lua_tonumber(LuaState,-1);
}

string CLuaObject::getgString( const char* Key )
{
  lua_getglobal(LuaState, Key);
  return (string)lua_tostring(LuaState,-1);
}

