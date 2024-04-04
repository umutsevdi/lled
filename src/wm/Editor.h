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
namespace lled
{
class Editor : public Window {
   public:
    Editor() : Window("Text Editor", ImGuiWindowFlags_NoNav, true) {}

    Editor(const Editor&) = delete;
    Editor(Editor&&) = delete;
    ~Editor(){};

    struct {
        bool active;
        size_t begin;
        size_t end;
    } selection;

    void menu_bar_function() override;
    void body() override;

   private:
    char buffer[2048];
    bool show_output = true;
    lled::Status result = {};
};

}// namespace lled
