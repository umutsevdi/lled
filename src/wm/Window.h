#pragma once
#include <string>

extern bool lled_is_ready();

#include "imgui.h"
namespace lled
{

class Window {
   public:
    Window(std::string _name, int _flag, bool _menu_bar)
        : menu_bar(_menu_bar), name(_name), flag(_flag)
    {
        if (!lled_is_ready()) { throw "Illegal State"; }
        if (this->menu_bar) { this->flag |= ImGuiWindowFlags_MenuBar; }
    }
    virtual ~Window();

    Window(Window&&) = default;
    Window(const Window&) = default;
    Window& operator=(Window&&) = delete;
    Window& operator=(const Window&) = delete;

    void context();

   protected:
    virtual void menu_bar_function() = 0;
    /* Draws the window interface */
    virtual void body() = 0;

    bool menu_bar;
    std::string name;
    int flag;
};

}// namespace lled
