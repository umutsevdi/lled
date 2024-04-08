#include "wm/LuaEditor.h"

void lled::LuaEditor::menu_bar_function()
{
    TextEditor::menu_bar_function();
    ImGui::MenuItem("Menu");

    if (ImGui::MenuItem("Execute")) {

        auto ctx = std::string_view(this->get_buffer());
        if (this->selection.active) {
            ctx = ctx.substr(selection.begin, selection.end);
        }
        result = shell.exec(ctx);
        selection = {false, 0, 0};
    }
}

void msg_handler(lled::Status* status)
{
    if (ImGui::BeginPopup("Result")) {
        ImGui::TextColored(ImVec4(200, 200, 0, 255), "Result: %s",
                           status->msg.c_str());
        ImGui::Separator();
        if (ImGui::Button("Ok")) {
            ImGui::CloseCurrentPopup();
            *status = lled::Status::OK();
        }
        ImGui::EndPopup();
    }
}

void lled::LuaEditor::body()
{
    TextEditor::body();
    if (result.is_ok() && !result.msg.empty()) { ImGui::OpenPopup("Result"); }
    msg_handler(&this->result);
}
