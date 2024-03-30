#include "lang/lang.h"

lang::Lua::Lua()
{
    this->state = luaL_newstate();
    luaL_openlibs(this->state);
}

lang::Lua::~Lua() { lua_close(this->state); }

void lang::Lua::loop()
{
    int error;
    std::string line = "";
    std::cin >> line;
    while (line != "quit") {
        error =
            luaL_loadbuffer(this->state, line.c_str(), line.length(), "line")
            || lua_pcall(this->state, 0, 0, 0);
        if (error) {
            std::cerr << lua_tostring(this->state, -1) << std::endl;
            lua_pop(this->state, 1); /* pop error message from the stack */
            error = 0;
        }
        std::cin >> line;
    }
    std::cout << "Lua runtime exited" << std::endl;
}
