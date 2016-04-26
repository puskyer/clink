// Copyright (c) 2015 Martin Ridgers
// License: http://opensource.org/licenses/MIT

#include "pch.h"
#include "lua_state.h"
#include "lua_script_loader.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

//------------------------------------------------------------------------------
void clink_lua_initialise(lua_State*);
void os_lua_initialise(lua_State*);
void path_lua_initialise(lua_State*);
void settings_lua_initialise(lua_State*);

//------------------------------------------------------------------------------
lua_state::lua_state()
: m_state(nullptr)
{
    initialise();
}

//------------------------------------------------------------------------------
lua_state::~lua_state()
{
    shutdown();
}

//------------------------------------------------------------------------------
void lua_state::initialise(bool use_debugger)
{
    shutdown();

    // Create a new Lua state.
    m_state = luaL_newstate();
    luaL_openlibs(m_state);

    clink_lua_initialise(m_state);
    os_lua_initialise(m_state);
    path_lua_initialise(m_state);
    settings_lua_initialise(m_state);

    if (use_debugger)
        lua_load_script(m_state, lib, debugger);
}

//------------------------------------------------------------------------------
void lua_state::shutdown()
{
    if (m_state == nullptr)
        return;

    lua_close(m_state);
    m_state = nullptr;
}

//------------------------------------------------------------------------------
bool lua_state::do_string(const char* string)
{
    if (luaL_dostring(m_state, string))
        return false;

    return true;
}

//------------------------------------------------------------------------------
bool lua_state::do_file(const char* path)
{
    if (luaL_dofile(m_state, path))
        return false;

    return true;
}