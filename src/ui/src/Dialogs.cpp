#include "ui/Dialogs.h"
#include "imgui.h"

void lled::popup_tooltip(Result& r, bool& show)
{
    if (!show) { return; }
    bool pressed = false;
    if (ImGui::BeginPopup(r.ok ? "Result" : "Error")) {
        if (r.ok) {

            ImGui::TextColored(ImVec4(200, 200, 0, 255), "Result: %s",
                               r.msg.c_str());
        } else {
            ImGui::TextColored(ImVec4(200, 0, 0, 255), "Error: %s",
                               r.msg.c_str());
        }
        ImGui::Separator();
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
            pressed = true;
        }
        ImGui::EndPopup();
    };
    show = !pressed;
}
