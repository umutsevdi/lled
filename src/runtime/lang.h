#pragma once

/******************************************************************************

 * File: src/lang/lang.h
 *
 * Author: Umut Sevdi
 * Created: 03/30/24
 * Description: This header for language utils

*****************************************************************************/
#include <iostream>
namespace lua
{
class Runtime {
   public:
    static Runtime& instance();

    /**
     * Creates an interactive shell for Lua programming language
     */
    void shell();
    /* Returns the Lua version */
    int version();

    Runtime& operator=(Runtime&&) = default;
    Runtime& operator=(const Runtime&) = default;
    friend std::ostream& operator<<(std::ostream& s, const Runtime& l)
    {
        s << l.instance().version();
        return s;
    }

   private:
    Runtime();
    ~Runtime();
};
}// namespace lua
