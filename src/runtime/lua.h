#pragma once

/******************************************************************************

 * File: src/runtime/lua.h
 *
 * Author: Umut Sevdi
 * Created: 03/30/24
 * Description: This header for language utils

*****************************************************************************/
#include <iostream>
#include <sstream>
#include <vector>
namespace lled
{

class Status {
   public:
    Status() : code(0), ok(true) {}
    Status(int _code, std::string _msg)
        : code(_code), msg(_msg), ok(_code == 0){};
    ~Status() = default;

    Status& operator=(const Status& s)
    {
        code = s.code;
        msg = s.msg;
        ok = s.ok;
        return *this;
    }
    int code;
    std::string msg;
    bool ok;
};

class Lua {
   public:
    static Lua& instance();
    /* Returns the Lua version */
    int version();

    Lua& operator=(Lua&&) = default;
    Lua& operator=(const Lua&) = delete;
    bool lock = false;

   private:
    Lua();
    ~Lua();
};

class Shell {
   public:
    Shell() : lua(Lua::instance()) {}
    ~Shell() {}

    /* Creates an interactive shell until "quit" or "exit" is called */
    void shell();
    /**
     * Executes the given statement returns an error code if there is any
     * @statement - to execute
     * @log - whether to print error messages to stderr
     * @return error code
     */
    Status exec(std::string_view statement);

    /**
     * Executes the given list of statements and returns an error code if there
     * is any
     * @statement - to execute
     * @log - whether to print error messages to stderr
     * @return error code
     */
    Status exec(std::vector<std::string>& statement);

    Shell& operator=(Shell&&) = delete;
    Shell& operator=(const Shell&) = delete;

   private:
    Lua& lua;
    std::stringstream _output;
};
}// namespace lled
