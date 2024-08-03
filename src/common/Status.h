#pragma once
/******************************************************************************

 * File: src/common/Status.h
 *
 * Author: Umut Sevdi
 * Created: 04/06/24
 * Description: A class that returns whether given function has succeeded or not

*****************************************************************************/
#include <complex>
#include <string>

namespace lled
{
inline constexpr size_t abs_dif(size_t a, size_t b)
{
    return a > b ? a - b : b - a;
}

class Result {
   public:
    bool ok;
    std::string msg;
    Result(bool r) : ok(r) {}
    Result(bool _ok, std::string _msg) : ok(_ok), msg(_msg) {}
    Result(Result&&) = default;
    Result(const Result&) = default;
    Result& operator=(Result&&) = default;
    Result& operator=(const Result&) = default;
    ~Result() = default;

    static Result OK(std::string msg = "") { return Result(true, msg); }
    static Result ERROR(std::string msg = "") { return Result(false, msg); }

   private:
    friend std::ostream& operator<<(std::ostream& os, Result& r)
    {
        if (!r.ok) { os << r.msg; }
        return os;
    }
};

}// namespace lled
