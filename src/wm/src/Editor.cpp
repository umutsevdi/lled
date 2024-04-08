#include "wm/Editor.h"
#include "common/Status.h"
#include "imgui.h"
#include "tinyfiledialogs.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

lled::Status lled::TextEditor::open_file()
{
    char* path = tinyfd_openFileDialog("Open File", "", 0, nullptr, "", false);
    // open_file is cancelled
    if (path == nullptr) { return Status::OK(); }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return Status::ERROR("Failed to open the file at " + std::string(path));
    }
    file.seekg(0, std::ios::end);
    std::streampos size = file.tellg();
    file.seekg(0, std::ios::beg);
    this->buffer.resize(size);
    file.read(buffer.data(), size);
    buffer.data()[size] = '\0';
    this->saved = true;
    this->path = path;
    this->name = this->base_name + " | " + path;

    file.close();
    return Status::OK();
}

lled::Status lled::TextEditor::save_file() { return lled::Status::OK(); }
lled::Status lled::TextEditor::save_file_as() { return lled::Status::OK(); }

int lled::TextEditor::text_input_cb(ImGuiInputTextCallbackData* data)
{
    lled::TextEditor* editor = (lled::TextEditor*)data->UserData;
    if (editor->selection.active = data->HasSelection(),
        editor->selection.active) {
        editor->selection.active = true;
        editor->selection.begin = MIN(data->SelectionStart, data->SelectionEnd);
        editor->selection.end = MAX(data->SelectionStart, data->SelectionEnd);
    }
    editor->cursor = data->CursorPos;

    if ((size_t)data->CursorPos > editor->get_buffer_s() * 8 / 10) {
        editor->append_buffer_position(data->CursorPos);
    }
    return 0;
}

void lled::TextEditor::menu_bar_function()
{
    if (this->saved) {
        this->flag &= ~(size_t)ImGuiWindowFlags_UnsavedDocument;
    } else {
        this->flag |= (size_t)ImGuiWindowFlags_UnsavedDocument;
    }

    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("New")) {
            if (this->saved) {
                this->saved = false;
                this->append_buffer_position(1024);
                this->path = "";
                this->name = this->base_name;
            }
        }
        if (ImGui::MenuItem("Open")) {
            if (!this->saved) {
                if (this->path == "") { save_file(); }
            }
            if (Status s = open_file(); !s.is_ok()) { this->result = s; }
        }

        if (ImGui::MenuItem("Save")) {
            if (!this->saved) {
                if (this->path != "") { save_file(); }
            }
        }
        ImGui::MenuItem("Save As");
        ImGui::MenuItem("Reload");

        if (ImGui::MenuItem("Close")) {
            if (this->saved) { delete this; }
        };
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {

        ImGui::MenuItem("Find");
        ImGui::MenuItem("Replace");
        ImGui::MenuItem("Line Numbers");

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Window")) {

        ImGui::MenuItem("Wrap Lines");
        ImGui::MenuItem("Tabs/Spaces");
        ImGui::MenuItem("Save");
        ImGui::MenuItem("Save");
        ImGui::MenuItem("Save");

        ImGui::EndMenu();
    }
}

size_t lled::TextEditor::get_new_lines(size_t* idx)
{
    size_t nl = 0;
    for (size_t i = 0; i < this->cursor; i++) {
        if (this->buffer[i] == '\n') {
            nl++;
            *idx = i;
        }
    }
    if (nl == 0) { *idx = this->buffer.size(); }
    return nl;
}

void lled::TextEditor::body()
{
    if (this->buffer.size()) {
        size_t r_idx = 0;
        size_t c_idx = get_new_lines(&r_idx);
        r_idx = r_idx == this->buffer.size() ? this->cursor
                                             : this->cursor - r_idx - 1;
        ImGui::TextColored(ImVec4(128.0, 128.0, 128.0, 255.0),
                           "ROW:%-4zu|COL:%-4zu|IDX:%-4zu/%-4zu", r_idx, c_idx,
                           this->cursor, this->buffer.size());
        if (this->selection.active) {
            ImGui::SameLine();
            ImGui::SetCursorPosX(w_end(true, IMGUI_FONT_SIZE_X * 8));
            ImGui::TextColored(ImVec4(128.0, 128.0, 128.0, 255.0), "SLC:%-4zu",
                               selection.end - selection.begin);
        }
        ImGui::InputTextMultiline("##text", this->get_buffer(),
                                  this->get_buffer_s(),
                                  ImVec2(w_end(1) - 2 * IMGUI_FONT_SIZE_X,
                                         w_end(0) - 5 * IMGUI_FONT_SIZE_Y),
                                  ImGuiInputTextFlags_CallbackAlways,
                                  &TextEditor::text_input_cb, this);

    } else {
        ImGui::SetCursorPosX(w_split(1, 2, IMGUI_FONT_SIZE_X * 37));
        ImGui::SetCursorPosY(w_split(0, 2, IMGUI_FONT_SIZE_Y * 2));

        ImGui::TextColored(ImVec4(0.0, 200.0, 200.0, 255.0),
                           "Create a file using File>New or\n"
                           "open an existing file from File>Open.");
    }
    if (!result.is_ok()) { ImGui::OpenPopup("Error"); }
    this->window_error("Error", &this->result);
}
