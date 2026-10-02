#ifndef LOG_HPP
#define LOG_HPP

#include <cstdarg>

#define LOG_VERBOSE 0

namespace bolt
{

    void log_info(const char* format, ...);
    void log_warning(const char* format, ...);
    void log_error(const char* format, ...);
    void log_debug(const char* format, ...);

} // namespace

#endif // LOG_HPP
