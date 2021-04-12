#include "log.hpp"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define MESSAGE_SIZE 500

namespace mirror {
	void log_impl(const char* color, const char* type, const char* format, ...)
	{
		va_list args;
		char msgBuff[MESSAGE_SIZE];
		int offset = 0;
		int writeSize = 0;
		int endSize = strlen("\033[0m");

		// Color, type
		writeSize = sprintf(msgBuff, "%s[%5s] ", color, type);
		if (writeSize < 0) {
			return;
		}
		offset += writeSize;

		// Message
		va_start(args, format);
		writeSize = vsprintf(msgBuff + offset, format, args);
		va_end(args);

		if (writeSize < 0) {
			return;
		}
		offset += writeSize;

		// End
		strncat(msgBuff + offset, "\033[0m", endSize);
		offset += endSize;

		// Printing
		printf("%s\n", msgBuff);
	}
}