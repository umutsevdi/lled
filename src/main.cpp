#include "runtime/lua.h"
#include "wm/Editor.h"
#include "wm/display.h"
#include <iostream>
#include <thread>

void setup()
{
    auto& instance = lled::WindowManager::instance();
    lled::Editor editor;
    instance.loop([&]() { editor.context(); });
    std::cout << "Hello gl";
}

int main(int argc, char* argv[])
{
    lled::Lua& l = lled::Lua::instance();
    std::cout << l << std::endl;
    std::cout << "alive ";
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i];
    }
    std::cout << std::endl;
    std::thread t([]() { setup(); });
    l.shell();
    return 0;
}
