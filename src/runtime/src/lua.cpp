#include "runtime/lua.h"
#include <iostream>
#include <numeric>
#include <string>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* Internal Lua state */
lua_State* _state;
/* Address of the current string stream */
std::stringstream* _strstream;

int _io_write(lua_State* L)
{
    int n = lua_gettop(L);
    for (int i = 1; i <= n; i++) {
        const char* s = luaL_checkstring(L, i);
        if (s == NULL)
            return luaL_error(L, "argument to 'output' must be a string");
        if (_strstream != nullptr) *_strstream << s;
    }

    return 0;
}

int _print(lua_State* L)
{
    int n = lua_gettop(L);
    for (int i = 1; i <= n; i++) {
        const char* s = luaL_tolstring(L, i, NULL);
        if (_strstream != nullptr) *_strstream << s << "\t";
        lua_pop(L, 1);
    }
    if (_strstream != nullptr) *_strstream << std::endl;

    return 0;
}

void override_io_write()
{
    lua_getglobal(_state, "io");
    lua_pushcfunction(_state, _io_write);
    lua_setfield(_state, -2, "write");
    lua_register(_state, "print", _print);
    luaL_dostring(_state, "print('Lua runtime is enabled')");
}

/**
 * Calculates the absolute indentation by counting tokens within the given line,
 * returns it.
 * @line - to count
 * @return - indentation count
 */
inline int _count_stack(std::string& line);

lled::Lua::Lua()
{
    _state = luaL_newstate();
    luaL_openlibs(_state);
    override_io_write();
}

lled::Lua::~Lua() { lua_close(_state); }

lled::Lua& lled::Lua::instance()
{
    static Lua instance;
    return instance;
}

int lled::Lua::version() { return lua_version(_state); }

void lled::Shell::shell()
{
    int syntax_stack = 0;
    std::string line;
    std::vector<std::string> buffer = {};
    while (true) {
        std::cout << ">> ";
        for (int i = 0; i < syntax_stack; i++) {
            std::cout << "    ";
        }
        std::getline(std::cin, line);
        if (line == "quit" || line == "exit") {
            break;
        } else {
            buffer.push_back(line + "\n");
            syntax_stack += _count_stack(line);
        }
        if (!syntax_stack) {
            Status s = exec(buffer);
            if (!s.is_ok()) {
                std::cerr << "Error: " << s.msg << std::endl;
            } else {
                std::cout << s.msg << std::endl;
            }
            buffer.clear();
        }
    }
}

lled::Status lled::Shell::exec(std::string_view statement)
{
    while (this->lua.lock) {}
    _output.str("");
    this->lua.lock = true;
    _strstream = &this->_output;
    int err = luaL_loadbuffer(_state, statement.data(), statement.length(), "")
              || lua_pcall(_state, 0, 0, 0);
    if (err) {
        std::string code = lua_tostring(_state, -1);
        lua_pop(_state, 1);
        this->lua.lock = false;
        return lled::Status::ERROR(code);
    }
    this->lua.lock = false;
    return Status::OK(this->_output.str());
}

lled::Status lled::Shell::exec(std::vector<std::string>& statement)
{
    return exec(
        std::accumulate(statement.begin(), statement.end(), std::string()));
}

/* Returns the number of occurrences of the needle within the line */
static inline int _count(const std::string& line, const std::string& needle)
{
    int n = 0;
    std::string ::size_type pos = 0;
    while ((pos = line.find(needle, pos)) != std::string::npos) {
        n++;
        pos += needle.size();
    }
    return n;
}

/**
 * "function", "do" are block indentation in Lua
 * "then" is a block indentation except when used with "elseif"
 * "end" closes existing block */
inline int _count_stack(std::string& line)
{
    return _count(line, "function") + _count(line, "do") + _count(line, "then")
           - _count(line, "elseif") - _count(line, "end");
}
