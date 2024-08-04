#pragma once
#include <string>

#include "common/Status.h"
#include "ui/Display.h"
namespace lled
{

class Window {
   public:
    Window(std::string _name, int _flag, bool _menu_bar);
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
    float w_end(bool axis = true, float object_size = 0);

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
    bool menu_bar;
    std::string name;
    int flag;
    bool enabled = true;
};

}// namespace lled
