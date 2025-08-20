#include "common/log.hpp"

#include <cstdio>
#include <format>
#include <string>
#include <string_view>

#include <unistd.h>

#include "common/iequal.hpp"

namespace craie::log {
namespace {

void format_level(std::string& buf, Level level) {
    switch (level) {
    case Level::trace:
        buf.append(terminal_output() ? "\033[94m" : "<7>");
        buf.append("TRACE: ");
        break;
    case Level::debug:
        buf.append(terminal_output() ? "\033[36m" : "<7>");
        buf.append("DEBUG: ");
        break;
    case Level::info:
        break;
    case Level::warning:
        buf.append(terminal_output() ? "\033[1;93m" : "<4>");
        buf.append("WARNING: ");
        break;
    case Level::error:
        buf.append(terminal_output() ? "\033[1;91m" : "<3>");
        buf.append("ERROR: ");
        break;
    case Level::dev:
        buf.append(terminal_output() ? "\033[1;95m" : "<5>");
        buf.append("DEV: ");
        break;
    }
}

Level& verbosity_mutable() {
    static Level r = [] {
        if (const char* env = std::getenv("LOGLEVEL"); env) {
            if (iequal(env, "trace"))
                return Level::trace;
            if (iequal(env, "debug"))
                return Level::debug;
            if (iequal(env, "warning"))
                return Level::warning;
            if (iequal(env, "error"))
                return Level::error;
        }
        return Level::info;
    }();
    return r;
}

}  // namespace


bool terminal_output() {
    static const bool r = [] {
        int fd = fileno(stderr);
        if (fd == -1)
            return false;
        return isatty(fd) == 1;
    }();
    return r;
}

Level verbosity() {
    return verbosity_mutable();
}

void set_verbosity(Level level) {
    verbosity_mutable() = level;
}


bool vlog(Level level, std::string_view fmt, const std::format_args& args) {
    if (level < verbosity())
        return false;

    std::string buf;
    format_level(buf, level);
    std::vformat_to(std::back_inserter(buf), fmt, args);
    if (terminal_output())
        buf.append("\033[0m");
    buf.push_back('\n');

    (void)std::fwrite(buf.data(), 1, buf.size(), stderr);

    return true;
}

}  // namespace craie::log
