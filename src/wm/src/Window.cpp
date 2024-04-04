#include "wm/Window.h"

void lled::Window::context()
{

    ImGui::Begin(this->name.c_str(), nullptr, this->flag);
    if (this->menu_bar) {
        ImGui::BeginMenuBar();
        this->menu_bar_function();
        ImGui::EndMenuBar();
    }
    this->body();
    ImGui::End();
}
lled::Window::~Window() {}
