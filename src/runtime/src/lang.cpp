#include "runtime/lang.h"
#include <numeric>
#include <string>
#include <vector>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

lua_State* _state;

/**
 * Executes given text block
 * @state - lua state
 * @block - text block
 * @return - error code
 */
int _interpret_statement(lua_State* state, std::string block);

/**
 * Counts the indentation modifying changing tokens within the
 * given line, returns the stack indentation.
 * @line - to count
 * @return - indentation count
 */
int count_stack(std::string& line);

lua::Runtime::Runtime()
{
    _state = luaL_newstate();
    luaL_openlibs(_state);
}

lua::Runtime::~Runtime() { lua_close(_state); }

lua::Runtime& lua::Runtime::instance()
{
    static Runtime instance;
    return instance;
}

int lua::Runtime::version() { return lua_version(_state); }

void lua::Runtime::shell()
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
            syntax_stack += count_stack(line);
        }
        if (!syntax_stack) {
            std::cout << "Executing" << std::endl;
            _interpret_statement(
                _state,
                std::accumulate(buffer.begin(), buffer.end(), std::string()));
            buffer.clear();
        }
    }
    std::cout << "Lua runtime exited" << std::endl;
}

int _interpret_statement(lua_State* state, std::string block)
{
    int error = luaL_loadbuffer(state, block.c_str(), block.length(), "line")
                || lua_pcall(state, 0, 0, 0);
    if (error) {
        std::cerr << lua_tostring(state, -1) << std::endl;
        lua_pop(state, 1); /* pop error message from the stack */
    }
    return error;
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

int count_stack(std::string& line)
{
    int stack = 0;
    stack += _count(line, "function");// do is a block indentation in Lua
    stack += _count(line, "do");      // do is a block indentation in Lua
    stack += _count(line, "then");    // then is a block indentation in Lua
    stack -= _count(line, "elseif");// except for when then is used with elseif
    stack -= _count(line, "end");   // end closes blocks in Lua
    return stack;
}
