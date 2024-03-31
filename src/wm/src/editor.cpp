#include "wm/editor.h"
#include "imgui.h"
#include "runtime/lua.h"
#include "wm/display.h"

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

    return 0;
}

lled::Editor::Editor()
{
    if (!lled::is_ready()) { throw "Illegal State"; }
}

lled::Editor::~Editor() {}

lled::Editor& lled::Editor::instance()
{
    static Editor instance;
    return instance;
}

void lled::Editor::context()
{
    ImGui::Begin("Text Editor", nullptr,
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoNav);
    ImGui::BeginMenuBar();
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
        auto& lua = Lua::instance();
        auto ctx = std::string(buffer);
        if (this->selection.active) {
            ctx = ctx.substr(selection.begin, selection.end);
        }
        result = lua.exec(ctx, true);
        selection = {false, 0, 0};
    }
    ImGui::EndMenuBar();
    ImGui::TextColored(
        ImVec4(128.0, 128.0, 128.0, 255.0), "selected: %s %zu %zu %s",
        selection.active ? "true" : "false", selection.begin, selection.end,
        std::string(buffer).substr(selection.begin, selection.end).c_str());

    ImGui::InputTextMultiline(
        "##text", buffer, 2048, ImVec2(ImGui::GetWindowSize().x, 250),
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

    ImGui::End();
}
