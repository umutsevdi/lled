#include "runtime/lua.h"
#include <numeric>
#include <string>
#include <vector>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

lua_State* _state;
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

/* Returns the number of occurrences of the needle within the line */
int _count(const std::string& line, const std::string& needle);

/**
 * Calculates the absolute indentation by counting tokens within the given line,
 * returns it.
 * @line - to count
 * @return - indentation count
 */
int _count_stack(std::string& line);

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
            if (!s.ok) {
                std::cerr << "Error: " << s.msg << std::endl;
            } else {
                std::cout << s.msg << std::endl;
            }
            buffer.clear();
        }
    }
    std::cout << "Lua runtime exited" << std::endl;
}

lled::Status lled::Shell::exec(std::string_view statement)
{
    while (this->lua.lock) {}
    this->lua.lock = true;
    _strstream = &this->_output;
    int err = luaL_loadbuffer(_state, statement.data(), statement.length(), "")
              || lua_pcall(_state, 0, 0, 0);
    if (err) {
        std::string code = lua_tostring(_state, -1);
        lua_pop(_state, 1);
        this->lua.lock = false;
        return lled::Status(err, code);
    }
    Status s(0, this->_output.str());
    _output.flush();
    this->lua.lock = false;
    return s;
}

lled::Status lled::Shell::exec(std::vector<std::string>& statement)
{
    return exec(
        std::accumulate(statement.begin(), statement.end(), std::string()));
}

int _count(const std::string& line, const std::string& needle)
{
    int n = 0;
    std::string ::size_type pos = 0;
    while ((pos = line.find(needle, pos)) != std::string::npos) {
        n++;
        pos += needle.size();
    }
    return n;
}

int _count_stack(std::string& line)
{
    int stack = _count(line, "function");// do is a block indentation in Lua
    stack += _count(line, "do");         // do is a block indentation in Lua
    stack += _count(line, "then");       // then is a block indentation in Lua
    stack -= _count(line, "elseif");// except for when then is used with elseif
    stack -= _count(line, "end");   // end closes blocks in Lua
    return stack;
}
