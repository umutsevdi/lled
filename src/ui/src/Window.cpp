#include "ui/Window.h"
#include "common/Status.h"

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

void lled::Window::window_error(std::string error_name, Result* status)
{
    if (ImGui::BeginPopup(error_name.c_str())) {
        ImGui::TextColored(ImVec4(200, 0, 0, 255), "Error: %s",
                           status->msg.c_str());
        ImGui::Separator();
        if (ImGui::Button("Ok")) {
            ImGui::CloseCurrentPopup();
            *status = Result::OK();
        }
        ImGui::EndPopup();
    }
}

void lled::Window::window_error(std::string error_name)
{
    window_error(error_name, &this->result);
}

lled::Window::~Window() {}
