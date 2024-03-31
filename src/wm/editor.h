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

    struct {
        bool active;
        size_t begin;
        size_t end;
    } selection;

   private:
    Editor();
    ~Editor();

    char buffer[2048];
    bool show_output = true;
    lled::Status result = {};
};

}// namespace lled
