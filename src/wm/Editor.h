#pragma once
/******************************************************************************

 * File: wm/editor.h
 *
 * Author: Umut Sevdi
 * Created: 03/31/24
 * Description: 

*****************************************************************************/

#include "runtime/lua.h"
#include "wm/Window.h"
#include <vector>
namespace lled
{
class Editor : public Window {
   public:
    Editor()
        : Window("Text Editor", ImGuiWindowFlags_NoNav, true), _buffer(2048)
    {}

    Editor(const Editor&) = delete;
    Editor(Editor&&) = delete;
    ~Editor(){};

    struct {
        bool active;
        size_t begin;
        size_t end;
    } selection;
    struct {
        size_t row;
        size_t col;
        size_t idx;
    } cursor;

    char* get_buffer() { return _buffer.data(); };
    size_t get_buffer_s()
    {
        if (this->_buffer.size() > this->_buffer.max_size() * 2 / 3) {
            this->_buffer.reserve(this->_buffer.max_size() * 2);
        }
        return _buffer.capacity();
    }
    void menu_bar_function() override;
    void body() override;

   private:
    std::vector<char> _buffer;
    bool show_output = true;
    lled::Status result = {};
    lled::Shell shell;
};

}// namespace lled
