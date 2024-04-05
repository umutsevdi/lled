#include "wm/Editor.h"
#include <string_view>

int _text_input_cb(ImGuiInputTextCallbackData* data)
{
    lled::Editor* editor = (lled::Editor*)data->UserData;
    bool selection = data->HasSelection();
    if (selection) {
        editor->selection.active = true;
        if (data->SelectionStart > data->SelectionEnd) {
            editor->selection.begin = static_cast<size_t>(data->SelectionEnd);
            editor->selection.end = static_cast<size_t>(data->SelectionStart);
        } else {
            editor->selection.begin = static_cast<size_t>(data->SelectionStart);
            editor->selection.end = static_cast<size_t>(data->SelectionEnd);
        }
    } else {
        editor->selection = {.active = false};
    }
    editor->cursor.idx = data->CursorPos;

    return 0;
}

void lled::Editor::menu_bar_function()
{
    if (ImGui::BeginMenu("File")) {
        ImGui::MenuItem("Open");
        ImGui::MenuItem("Save");
        ImGui::MenuItem("Close");
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
        if (ImGui::MenuItem("Toggle Console")) {
            this->show_output = !this->show_output;
        };
        ImGui::EndMenu();
    }
    ImGui::MenuItem("Menue");

    if (ImGui::MenuItem("Execute")) {

        auto ctx = std::string_view(this->get_buffer());
        if (this->selection.active) {
            ctx = ctx.substr(selection.begin, selection.end);
        }
        result = shell.exec(ctx);
        selection = {false, 0, 0};
    }
}

void lled::Editor::body()
{
    ImGui::TextColored(ImVec4(128.0, 128.0, 128.0, 255.0),
                       "r:%zu | c:%zu | %zu/%zu selected: %s %zu %zu",
                       this->cursor.row, this->cursor.col, this->cursor.idx,
                       get_buffer_s(), selection.active ? "true" : "false",
                       selection.begin, selection.end);

    ImGui::InputTextMultiline(
        "##text", this->get_buffer(), this->get_buffer_s(),
        ImVec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y - 100),
        ImGuiInputTextFlags_CallbackAlways, _text_input_cb, this);
    if (show_output) {
        ImGui::TextColored(ImVec4(128, 200, 0, 255), "%s", result.msg.c_str());
    }
    if (!result.ok) { ImGui::OpenPopup("Error"); }
    if (ImGui::BeginPopup("Error")) {
        ImGui::Text("Code: %d", result.code);
        ImGui::TextColored(ImVec4(200, 0, 0, 255), "Error: %s",
                           result.msg.c_str());
        ImGui::Separator();
        if (ImGui::Button("Ok")) {
            ImGui::CloseCurrentPopup();
            result = Status();
        }
        ImGui::EndPopup();
    }
}
