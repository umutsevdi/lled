#include "runtime/lua.h"
#include "wm/display.h"
#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
    lled::Lua& l = lled::Lua::instance();
    std::cout << l << std::endl;
    std::cout << "alive ";
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i];
    }
    std::cout << std::endl;
    std::thread t([]() { lled::setup(); });
    l.shell();
    return 0;
}
