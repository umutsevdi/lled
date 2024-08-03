#pragma once

/******************************************************************************

 * File: src/runtime/lua.h
 *
 * Author: Umut Sevdi
 * Created: 03/30/24
 * Description: This header for language utils

*****************************************************************************/
#include "common/Status.h"
#include <sstream>
#include <vector>
namespace lled
{

namespace Lua
{
void start();
void end();
int version();

};// namespace Lua

class Shell {
   public:
    Shell() {}
    ~Shell() {}

    /* Creates an interactive shell until "quit" or "exit" is called */
    void shell();
    /**
     * Executes the given statement returns an error code if there is any
     * @statement - to execute
     * @log - whether to print error messages to stderr
     * @return error code
     */
    Result exec(std::string_view statement);

    /**
     * Executes the given list of statements and returns an error code if there
     * is any
     * @statement - to execute
     * @log - whether to print error messages to stderr
     * @return error code
     */
    Result exec(std::vector<std::string>& statement);

    Shell& operator=(Shell&&) = delete;
    Shell& operator=(const Shell&) = delete;

   private:
    std::stringstream _output;
};
}// namespace lled
