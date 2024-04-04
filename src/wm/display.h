#pragma once
/******************************************************************************

 * File: wm/display.h
 *
 * Author: Umut Sevdi
 * Created: 03/31/24
 * Description: 

*****************************************************************************/

bool lled_is_ready();
#include <functional>

namespace lled
{

class WindowManager {
   public:
    static WindowManager& instance();
    WindowManager& operator=(WindowManager&&) = default;
    WindowManager& operator=(const WindowManager&) = default;

    void loop(std::function<void(void)> fn);

   private:
    WindowManager();
    ~WindowManager();
};

}// namespace lled
