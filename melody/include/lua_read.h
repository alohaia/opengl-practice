extern "C"
{
#include <lauxlib.h>
#include <lualib.h>
#include <lua.h>
}
#include <cstdio>
#include <cstring>

namespace lua
{
    static bool CheckLua(lua_State* L, int r);

    lua_State * lua_init(const char * path);

    lua_State * lua_open(lua_State * L, const char * path);

    int get_number(lua_State* L, const char * name, double & rv);

    int get_string(lua_State* L, const char * name, char * rv);

    int get_bool(lua_State* L, const char * name, bool & rv);
}

static bool lua::CheckLua(lua_State* L, int r)
{
    if (r == LUA_OK)
        return true;
    else {
        printf(lua_tostring(L, -1));
        return false;
    }
}

lua_State * lua::lua_init(const char * path)
{
    lua_State * L = luaL_newstate();
    if (CheckLua(L, luaL_dofile(L, path)))
        return L;
    else
        return NULL;
}

lua_State * lua::lua_open(lua_State * L, const char * path)
{
    if (CheckLua(L, luaL_dofile(L, path)))
        return L;
    else
        return NULL;
}

int lua::get_number(lua_State* L, const char * name, double & rv)
{
    lua_getglobal(L, name);
    if (lua_isnumber(L, -1))
        rv = lua_tonumber(L, -1);
    else {
        printf("[lua::get_number]%s Not Found.\n",  name);
        return 2;
    }

    lua_pop(L, 1);
    return 0;
}

int lua::get_string(lua_State* L, const char * name, char * rv)
{
    lua_getglobal(L, name);
    if (lua_isstring(L, -1))
        strcpy(rv, lua_tostring(L, -1));
    else {
        printf("[lua::get_string]%s Not Found.\n",  name);
        return 2;
    }
    lua_pop(L, 1);
    return 0;
}

int lua::get_bool(lua_State* L, const char * name, bool & rv)
{
    lua_getglobal(L, name);
    if (lua_isboolean(L, -1))
        rv = lua_toboolean(L, -1);
    else {
        printf("[lua::get_string]%s Not Found.\n",  name);
        return 2;
    }
    lua_pop(L, 1);
    return 0;
}

