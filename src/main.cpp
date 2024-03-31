#include "runtime/lua.h"
#include "wm/display.h"
#include <iostream>
#include <thread>

int main(int argc, char* argv[])
{
    lua::Runtime& l = lua::Runtime::instance();
    std::cout << l << std::endl;
    std::cout << "alive ";
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i];
    }
    std::cout << std::endl;
    std::thread t([]() { gl::setup(); });
    l.shell();
    return 0;
}
