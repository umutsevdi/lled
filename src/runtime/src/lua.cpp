#include "runtime/lua.h"
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

lua_State* _state;

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
}

lled::Lua::~Lua() { lua_close(_state); }

lled::Lua& lled::Lua::instance()
{
    static Lua instance;
    return instance;
}

int lled::Lua::version() { return lua_version(_state); }

void lled::Lua::shell()
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
            Status s = exec_statement(buffer, true);
            if (!s.ok) {
                std::cerr << s.msg << std::endl;
            } else {
                std::cout << s.msg << std::endl;
            }
            buffer.clear();
        }
    }
    std::cout << "Lua runtime exited" << std::endl;
}

lled::Status lled::Lua::exec_statement(std::string& statement, bool log)
{
    int err = luaL_loadbuffer(_state, statement.c_str(), statement.length(), "")
              || lua_pcall(_state, 0, 0, 0);
    if (err) {
        std::string code = lua_tostring(_state, -1);
        lua_pop(_state, 1);
        if (log) { std::cerr << code << std::endl; }
        return lled::Status(err, code);
    }

    int nresults = lua_gettop(_state);
    std::stringstream s;
    for (int i = 1; i <= nresults; ++i) {
        const char* r = lua_tostring(_state, i);
        if (r != nullptr) { s << r << std::endl; }
    }
    return Status(0, s.str());
}

lled::Status lled::Lua::exec_statement(std::vector<std::string>& statement,
                                       bool log)
{
    std::string st =
        std::accumulate(statement.begin(), statement.end(), std::string());
    return exec_statement(st, log);
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
