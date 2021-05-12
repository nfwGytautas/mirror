#pragma once

namespace mirror {
	/**
	 * Actual use functions
	*/

#ifdef _MSC_VER
	#define log_error(fmt, ...) log_impl("\033[31m", "Error", fmt, __VA_ARGS__);
	#define log_warn(fmt, ...) log_impl("\033[33m", "Warn", fmt, __VA_ARGS__);
	#define log_info(fmt, ...) log_impl("\033[36m", "Info", fmt, __VA_ARGS__);
#else
    #define log_error(fmt, ...) log_impl("\033[31m", "Error", fmt, ## __VA_ARGS__);
    #define log_warn(fmt, ...) log_impl("\033[33m", "Warn", fmt, ## __VA_ARGS__);
    #define log_info(fmt, ...) log_impl("\033[36m", "Info", fmt, ## __VA_ARGS__);
#endif

	/**
	 * @brief Implementation of hogl logging function
	 * @param color ASCII color code
	 * @param type Text that is displayed before the message
	 * @param format Format of the message
	 * @param ... Format parameters
	*/
	void log_impl(const char* color, const char* type, const char* format, ...);
}