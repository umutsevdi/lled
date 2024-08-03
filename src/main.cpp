#include "runtime/lua.h"
#include "ui/Display.h"
#include "ui/Editor.h"
#include "ui/LuaEditor.h"
#include <csignal>
#include <iostream>
#include <thread>

void _init()
{
    lled::Lua::start();
    signal(SIGINT, [](int) -> void {
        lled::Lua::end();
        lled::wm::close();
    });
}

int main(int argc, char* argv[])
{
    _init();
    lled::Shell io;
    std::thread t([]() {
        lled::wm::init();
        lled::LuaEditor editor;
        lled::TextEditor feditor;
        lled::wm::loop([&]() {
            feditor.context();
            editor.context();
        });
        lled::wm::close();
    });
    io.shell();
    lled::Lua::end();
    return 0;
}
