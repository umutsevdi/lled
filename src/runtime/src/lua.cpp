#include "runtime/lua.h"
#include <iostream>
#include <numeric>
#include <string>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

struct lua_header_t {
    bool lock;
    /* Internal Lua state */
    lua_State* state;
    /* Address of the current string stream */
    std::stringstream* stream;
};

static lua_header_t t = {.lock = false};

/**
 * Pipes the Lua io functions to the string buffer
 */
void _override_io(void);

/**
 * Overload of write function
 */
int _io_write(lua_State* L);

/**
 * Overload of print function
 */
int _print(lua_State* L);

/**
 * Calculates the absolute indentation by counting tokens within the given line,
 * returns it.
 *
 * "function", "do" are block indentation in Lua
 * "then" is a block indentation except when used with "elseif"
 * "end" closes existing block 
 *
 * @line - to count
 * @return - indentation count
 */
inline int _count_stack(std::string& line);

void lled::Lua::start()
{
    t.state = luaL_newstate();
    luaL_openlibs(t.state);
    _override_io();
}

void lled::Lua::end() { lua_close(t.state); }

int lled::Lua::version() { return lua_version(t.state); }

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
            Result r = exec(buffer);
            if (!r.ok) {
                std::cerr << "Error: " << r.msg << std::endl;
            } else {
                std::cout << r.msg << std::endl;
            }
            buffer.clear();
        }
    }
}

lled::Result lled::Shell::exec(std::string_view statement)
{
    while (t.lock) {}
    _output.str("");
    t.lock = true;
    t.stream = &this->_output;
    int err = luaL_loadbuffer(t.state, statement.data(), statement.length(), "")
              || lua_pcall(t.state, 0, 0, 0);
    if (err) {
        std::string code = lua_tostring(t.state, -1);
        lua_pop(t.state, 1);
        t.lock = false;
        return Result::ERROR(code);
    }
    t.lock = false;
    return Result::OK(this->_output.str());
}

lled::Result lled::Shell::exec(std::vector<std::string>& statement)
{
    return exec(
        std::accumulate(statement.begin(), statement.end(), std::string()));
}

int _io_write(lua_State* L)
{
    int n = lua_gettop(L);
    for (int i = 1; i <= n; i++) {
        const char* s = luaL_checkstring(L, i);
        if (s == NULL)
            return luaL_error(L, "argument to 'output' must be a string");
        if (t.stream != nullptr) *t.stream << s;
    }

    return 0;
}

int _print(lua_State* L)
{
    int n = lua_gettop(L);
    for (int i = 1; i <= n; i++) {
        const char* s = luaL_tolstring(L, i, NULL);
        if (t.stream != nullptr) *t.stream << s << "\t";
        lua_pop(L, 1);
    }
    if (t.stream != nullptr) *t.stream << std::endl;

    return 0;
}

void _override_io()
{
    lua_getglobal(t.state, "io");
    lua_pushcfunction(t.state, _io_write);
    lua_setfield(t.state, -2, "write");
    lua_register(t.state, "print", _print);
    luaL_dostring(t.state, "print('Lua runtime is enabled')");
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

inline int _count_stack(std::string& line)
{
    return _count(line, "function") + _count(line, "do") + _count(line, "then")
           - _count(line, "elseif") - _count(line, "end");
}
