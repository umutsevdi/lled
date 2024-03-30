#include "lang/lang.h"
#include <iostream>

int main(int argc, char* argv[])
{
    lang::Lua& l = lang::Lua::instance();
    std::cout << l << std::endl;
    std::cout << "alive ";
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i];
    }
    std::cout << std::endl;
    l.loop();
    return 0;
}
