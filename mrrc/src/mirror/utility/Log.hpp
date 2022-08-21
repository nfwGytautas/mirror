#pragma once

#ifdef _MSC_VER
#define LOG_ERROR(fmt, ...) mirror::implementation::log_impl("\033[31m", "Error", fmt, __VA_ARGS__);
#define LOG_WARN(fmt, ...)  mirror::implementation::log_impl("\033[33m", "Warn", fmt, __VA_ARGS__);
#define LOG_INFO(fmt, ...)  mirror::implementation::log_impl("\033[36m", "Info", fmt, __VA_ARGS__);
#else
#define LOG_ERROR(fmt, ...) mirror::implementation::log_impl("\033[31m", "Error", fmt, ##__VA_ARGS__);
#define LOG_WARN(fmt, ...)  mirror::implementation::log_impl("\033[33m", "Warn", fmt, ##__VA_ARGS__);
#define LOG_INFO(fmt, ...)  mirror::implementation::log_impl("\033[36m", "Info", fmt, ##__VA_ARGS__);
#endif

namespace mirror {

namespace implementation {

/**
 * @brief Implementation of hogl logging function
 * @param color ASCII color code
 * @param type Text that is displayed before the message
 * @param format Format of the message
 * @param ... Format parameters
 */
void log_impl(const char* color, const char* type, const char* format, ...);

} // namespace implementation
} // namespace mirror
