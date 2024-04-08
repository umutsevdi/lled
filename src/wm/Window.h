#pragma once
#include <string>

#include "common/Status.h"
#include "imgui.h"
#include "wm/Display.h"
namespace lled
{
static const float IMGUI_FONT_SIZE_X = 3.5;
static const float IMGUI_FONT_SIZE_Y = 13;
#define MAX(a, b) (a) > (b) ? (a) : (b)
#define MIN(a, b) (a) > (b) ? (b) : (a)

class Window {
   public:
    Window(std::string _name, int _flag, bool _menu_bar)
        : menu_bar(_menu_bar), name(_name), flag(_flag)
    {
        if (!lled::is_ready()) { throw "Illegal State"; }
        if (this->menu_bar) { this->flag |= ImGuiWindowFlags_MenuBar; }
    }
    virtual ~Window();

    Window(Window&&) = default;
    Window(const Window&) = default;
    Window& operator=(Window&&) = delete;
    Window& operator=(const Window&) = delete;

    void context();

    /**
     * Returns the end of the window on given axis
     * @axis - true for x axis, false for y
     * @object_size - size of the object to align
     * @return end position
     */
    inline float w_end(bool axis = true, float object_size = 0)
    {
        return (axis ? ImGui::GetWindowWidth() : ImGui::GetWindowHeight())
               - object_size - 12;
    }

    inline float w_begin() { return 12; }

    inline float w_split(bool axis = true, int amount = 2,
                         float object_size = 0)
    {
        return (w_end(axis) - w_begin()) / amount - object_size;
    }

   protected:
    virtual void menu_bar_function() = 0;
    /* Draws the window interface */
    virtual void body() = 0;
    void window_error(std::string error_name);
    void window_error(std::string error_name, Status* status);

    bool menu_bar;
    std::string name;
    int flag;
    bool enabled = true;
    Status result = Status::OK();
};

}// namespace lled
