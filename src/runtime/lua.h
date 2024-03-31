#pragma once

/******************************************************************************

 * File: src/runtime/lua.h
 *
 * Author: Umut Sevdi
 * Created: 03/30/24
 * Description: This header for language utils

*****************************************************************************/
#include <iostream>
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

    /* Creates an interactive shell until "quit" or "exit" is called */
    void shell();

    /* Returns the Lua version */
    int version();

    /**
     * Executes the given statement returns an error code if there is any
     * @statement - to execute
     * @log - whether to print error messages to stderr
     * @return error code
     */
    Status exec_statement(std::string& statement, bool log = false);

    /**
     * Executes the given list of statements and returns an error code if there
     * is any
     * @statement - to execute
     * @log - whether to print error messages to stderr
     * @return error code
     */
    Status exec_statement(std::vector<std::string>& statement,
                          bool log = false);

    Lua& operator=(Lua&&) = default;
    Lua& operator=(const Lua&) = default;
    friend std::ostream& operator<<(std::ostream& s, const Lua& l)
    {
        s << l.instance().version();
        return s;
    }

   private:
    Lua();
    ~Lua();
};
}// namespace lled
