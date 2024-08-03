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

namespace wm
{
void init(std::string name = "lled", int width = 960, int height = 720);
void close();
void loop(std::function<void(void)> fn);

}// namespace wm
}// namespace lled
