/******************************************************************************

 * File: wm/editor.h
 *
 * Author: Umut Sevdi
 * Created: 03/31/24
 * Description: 

*****************************************************************************/

#include "runtime/lua.h"
namespace lled
{
class Editor {
   public:
    static Editor& instance();
    void context();

    Editor& operator=(Editor&&) = default;
    Editor& operator=(const Editor&) = default;

   private:
    Editor();
    ~Editor();

    char buffer[2048];
    lled::Status result = {};
};

}// namespace lled
