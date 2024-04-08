#pragma once
/******************************************************************************

 * File: src/wm/Editor.h
 *
 * Author: Umut Sevdi
 * Created: 04/06/24
 * Description: Generic Text editor

*****************************************************************************/

#include "wm/Window.h"
#include <vector>
namespace lled
{
class TextEditor : public Window {
   public:
    TextEditor(std::string _name = "Text Editor")
        : Window(_name, ImGuiWindowFlags_None, true), base_name(_name),
          buffer(0)
    {}

    TextEditor(const TextEditor&) = delete;
    TextEditor(TextEditor&&) = delete;
    ~TextEditor(){};

    struct {
        bool active;
        size_t begin;
        size_t end;
    } selection;
    size_t cursor;

    inline char* get_buffer() { return buffer.data(); };
    inline size_t get_buffer_s() { return this->buffer.capacity(); }
    inline void append_buffer_position(size_t cursor_position)
    {
        if (cursor_position * 5 / 4 > buffer.capacity()) {
            for (size_t i = 0; i < cursor_position / 4; i++)
                buffer.push_back('\0');
        }
    }
    void menu_bar_function() override;
    void body() override;

    /* Opens a Open File Dialog and writes its contents to the buffer. */
    Status open_file();
    /* Updates an already opened file's contents. */
    Status save_file();
    /* Opens a Save File Dialog to choose a new file path and then runs save_file function. */
    Status save_file_as();

   protected:
    std::string base_name = "Text Editor";
    std::string path;
    bool saved = true;
    std::vector<char> buffer;
    static int text_input_cb(ImGuiInputTextCallbackData* data);

   private:
    /**
     * Counts the number of new lines until the cursor's position
     * @c - the value that will be updated to the index of the last instance of 
     * new line
     * @return number of new lines in the buffer
     */
    size_t get_new_lines(size_t* c);
};

}// namespace lled
