#ifndef SPERO_BASE_EXCEPTION_H
#define SPERO_BASE_EXCEPTION_H

#include <string>
#include <exception>

namespace spero
{

class Exception : public std::exception
{
public:
    Exception(std::string what);
    ~Exception() noexcept override = default;

    // default copy-ctor and operator= are okay.

    const char* what() const noexcept override
    {
      return message_.c_str();
    }

    const char* stackTrace() const noexcept
    {
      return stack_.c_str();
    }

private:
    std::string message_;
    std::string stack_;
};

}  // namespace spero

#endif  // SPERO_BASE_EXCEPTION_H
