#include "ui/LuaEditor.h"
#include "ui/Dialogs.h"
#include <imgui.h>

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
        this->show = true;
        selection = {false, 0, 0};
    }
}

void lled::LuaEditor::body()
{
    TextEditor::body();
    if (show && result.ok && !result.msg.empty()) {
        ImGui::OpenPopup("Result");
    }
}
