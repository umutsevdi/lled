#pragma once
/******************************************************************************

 * File: src/common/Status.h
 *
 * Author: Umut Sevdi
 * Created: 04/06/24
 * Description: A class that returns whether given function has succeeded or not

*****************************************************************************/
#include <string>

namespace lled
{
inline constexpr size_t abs_dif(size_t a, size_t b)
{
    return a > b ? a - b : b - a;
}

class Status {
   public:
    static Status OK(std::string _msg = "") { return Status(0, _msg); }
    static Status ERROR(std::string msg) { return Status(1, msg); }

    ~Status() = default;

    Status& operator=(const Status& s)
    {
        code = s.code;
        msg = s.msg;
        return *this;
    }
    inline bool is_ok() { return code == 0; }
    int code;
    std::string msg;

   private:
    Status(int _code, std::string _msg) : code(_code), msg(_msg){};
};

class FileRepresentation {
   public:
};
}// namespace lled
