#pragma once
/******************************************************************************

 * File: wm/LuaEditor.h
 *
 * Author: Umut Sevdi
 * Created: 03/31/24
 * Description: Lua Editor

*****************************************************************************/

#include "runtime/lua.h"
#include "wm/Editor.h"
namespace lled
{
class LuaEditor : public TextEditor {
   public:
    LuaEditor() : TextEditor("Lua Editor") {}

    LuaEditor(const LuaEditor&) = delete;
    LuaEditor(LuaEditor&&) = delete;
    ~LuaEditor(){};

    void menu_bar_function() override;
    void body() override;

   private:
    bool show_output = true;
    Shell shell;
};

}// namespace lled
