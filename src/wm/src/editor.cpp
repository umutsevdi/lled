#include "wm/editor.h"
#include "imgui.h"
#include "runtime/lua.h"
#include "wm/display.h"
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
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse);
    ImGui::BeginMenuBar();
    ImGui::MenuItem("Item");
    ImGui::EndMenuBar();
    ImGui::Text("Hello");
    ImGui::SameLine();
    ImGui::InputTextMultiline("##text", buffer, IM_ARRAYSIZE(buffer),
                              ImVec2(-25.0f, -30.0f),
                              ImGuiInputTextFlags_AllowTabInput);
    if (ImGui::Button("Execute")) {
        auto& lua = Lua::instance();
        auto ctx = std::string(buffer);
        result = lua.exec_statement(ctx, true);
    }
    if (result.ok) {
        if (result.msg != "") {
            ImGui::TextColored(ImVec4(128, 200, 0, 255), "%s",
                               result.msg != "" ? result.msg.c_str()
                                                : "Statement run successfully."

            );
        }
    } else {
        ImGui::OpenPopup("Error");
    }
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
