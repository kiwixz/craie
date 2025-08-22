#pragma once

#include <format>
#include <stdexcept>
#include <system_error>

namespace craie {

struct Exception : std::runtime_error {
    template <typename... Args>
    Exception(std::format_string<Args...> format, Args&&... args) :
            std::runtime_error{std::format(format, std::forward<Args>(args)...)} {
    }
};


struct SystemException : std::system_error {
    template <typename... Args>
    SystemException(std::format_string<Args...> format, Args&&... args) :
            SystemException{errno, format, std::forward<Args>(args)...} {
    }

    template <typename... Args>
    SystemException(std::errc error_code, std::format_string<Args...> format, Args&&... args) :
            SystemException{static_cast<int>(error_code), format, std::forward<Args>(args)...} {
    }

    template <typename... Args>
    SystemException(int error_code, std::format_string<Args...> format, Args&&... args) :
            std::system_error{error_code, std::system_category(), std::format(format, std::forward<Args>(args)...)} {
    }
};

}  // namespace craie
