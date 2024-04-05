#pragma once
/******************************************************************************

 * File: wm/display.h
 *
 * Author: Umut Sevdi
 * Created: 03/31/24
 * Description: 

*****************************************************************************/

#include <functional>
#include <string>

namespace lled
{

bool is_ready();

class WindowManager {
   public:
    static WindowManager& instance();
    WindowManager& operator=(WindowManager&&) = default;
    WindowManager& operator=(const WindowManager&) = default;

    void loop(std::function<void(void)> fn);

   private:
    WindowManager(std::string _wm_name, int width = 960, int height = 720);
    ~WindowManager();

    std::string wm_name;
};

}// namespace lled
