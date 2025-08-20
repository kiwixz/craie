#pragma once

#include <cstdint>
#include <format>

namespace craie::log {

enum class Level : uint8_t {
    trace,
    debug,
    info,
    warning,
    error,
    dev,
};


bool terminal_output();
Level verbosity();
void set_verbosity(Level level);


bool vlog(Level level, std::string_view fmt, const std::format_args& args);

template <typename... Args>
bool log(Level level, std::format_string<Args...> fmt, Args&&... args) {
    return vlog(level, fmt.get(), std::make_format_args(args...));
}

template <typename... Args>
bool trace(std::format_string<Args...> fmt, Args&&... args) {
    return log(Level::trace, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool debug(std::format_string<Args...> fmt, Args&&... args) {
    return log(Level::debug, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool info(std::format_string<Args...> fmt, Args&&... args) {
    return log(Level::info, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool warn(std::format_string<Args...> fmt, Args&&... args) {
    return log(Level::warning, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool error(std::format_string<Args...> fmt, Args&&... args) {
    return log(Level::error, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
bool dev(std::format_string<Args...> fmt, Args&&... args) {
    return log(Level::dev, fmt, std::forward<Args>(args)...);
}

}  // namespace craie::log
