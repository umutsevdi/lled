#include "lang/lang.h"
#include <sstream>
#include <string>
#include <vector>

int _interpret_statement(lua_State* state, std::string block);

lang::Lua::Lua()
{
    this->state = luaL_newstate();
    luaL_openlibs(this->state);
}

lang::Lua::~Lua() { lua_close(this->state); }

void lang::Lua::loop()
{
    std::vector<std::string> buffer = {};
    std::string line;
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, line);
        if (line == "quit") {
            break;
        } else if (line == "") {
            std::stringstream s;
            for (std::string line : buffer) {
                s << line << std::endl;
            }
            _interpret_statement(this->state, s.str());
            buffer.clear();
        } else {
            buffer.push_back(line);
        }
    }
    std::cout << "Lua runtime exited" << std::endl;
}

int _interpret_statement(lua_State* state, std::string block)
{
    int error = 0;
    error = luaL_loadbuffer(state, block.c_str(), block.length(), "line")
            || lua_pcall(state, 0, 0, 0);
    if (error) {
        std::cerr << lua_tostring(state, -1) << std::endl;
        lua_pop(state, 1); /* pop error message from the stack */
        error = 0;
    }
    return error;
}
