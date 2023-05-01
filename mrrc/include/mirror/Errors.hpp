#pragma once

#include <cstdint>

namespace mirror {

/**
 * @brief Enum of possible compiler internal errors
 */
enum class CompilerInternalError : uint8_t {
    NO_ERROR = 0,             // Success
    BAD_ARGUMENTS = 1,        // Arguments passed to compiler are of incorrect format
    MISSING_SOURCE_FILES = 2, // No source files specified
};

} // namespace mirror
