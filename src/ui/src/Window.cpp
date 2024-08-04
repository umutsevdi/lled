#include "ui/Window.h"
#include "common/Status.h"

#include <imgui.h>

lled::Window::Window(std::string _name, int _flag, bool _menu_bar)
    : menu_bar(_menu_bar), name(_name), flag(_flag)

{
    if (!lled::is_ready()) { throw "Illegal State"; }
    if (this->menu_bar) { this->flag |= ImGuiWindowFlags_MenuBar; }
}

lled::Window::~Window() {}

void lled::Window::context()
{
    if (!this->enabled) { return; }
    ImGui::Begin(this->name.c_str(), nullptr, this->flag);
    if (this->menu_bar) {
        ImGui::BeginMenuBar();
        this->menu_bar_function();
        ImGui::EndMenuBar();
    }
    this->body();
    ImGui::End();
}

float lled::Window::w_end(bool axis, float object_size)
{
    return (axis ? ImGui::GetWindowWidth() : ImGui::GetWindowHeight())
           - object_size - 12;
}
