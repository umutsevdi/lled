#pragma once

/******************************************************************************

 * File: src/lang/lang.h
 *
 * Author: Umut Sevdi
 * Created: 03/30/24
 * Description: This header for language utils

*****************************************************************************/
#include <iostream>
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
namespace lang
{
class Lua {
   public:
    static Lua& instance()
    {
        static Lua instance;
        return instance;
    }
    void loop();

    Lua& operator=(Lua&&) = default;
    Lua& operator=(const Lua&) = default;
    friend std::ostream& operator<<(std::ostream& s, const lang::Lua& l)
    {
        s << "Lua runtime enabled";
        return s;
    }

   private:
    Lua();
    ~Lua();
    lua_State* state;
};
}// namespace lang
