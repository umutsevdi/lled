#include "runtime/lua.h"
#include "wm/Display.h"
#include "wm/Editor.h"
#include "wm/LuaEditor.h"
#include <iostream>
#include <thread>

void setup()
{
    auto& wm = lled::WindowManager::instance();
    lled::LuaEditor editor;
    lled::TextEditor feditor;
    wm.loop([&]() {
        feditor.context();
        editor.context();
    });
}

int main(int argc, char* argv[])
{
    lled::Shell io;
    std::cout << "alive ";
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i];
    }
    std::cout << std::endl;
    std::thread t([]() { setup(); });
    io.shell();
    return 0;
}
