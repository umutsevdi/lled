#include "wm/display.h"
#include "wm/editor.h"
#include <iostream>

void lled::setup()
{
    auto& instance = lled::WindowManager::instance();
    auto& editor = lled::Editor::instance();
    instance.instance();
    editor.instance();
    std::cout << "Hello gl";
}
