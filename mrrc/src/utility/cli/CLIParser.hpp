#pragma once

#include <string>
#include <vector>

#include "mirror/Errors.hpp"

namespace mirror::utility::cli {

/**
 * @brief Global options
 */
enum class GlobalOption : uint8_t {
    HELP,    // --help, -h
    VERSION, // --version, -v
};

/**
 * @brief Options for tokens command
 */
enum class TokensOptions : uint8_t {
    TRACE, // --trace, -t
};

/**
 * @brief Mirror compiler commands
 */
enum class Command : uint8_t {
    NO_COMMAND, // Command not specified
    TOKENS,     // Prints the mirror compiler generated tokens into a file
};

/**
 * @brief Utility class for parsing command line arguments
 */
class CLIParser final {
public:
    /**
     * @brief Construct a new CLIParser object
     *
     * @param argc Argument count
     * @param argv Arguments
     */
    CLIParser(int argc, char** argv);

private:
    /**
     * @brief React to a global option
     *
     * @param option Option
     * @param optionString String representation of the option
     */
    void reactToGlobalOption(GlobalOption option, const char* optionString);

    /**
     * @brief Reacts to a command option
     *
     * @param option Option
     */
    void reactToCommandOption(const std::string& option);

public:
    bool Empty = true; // False if there are any arguments passed

    CompilerInternalError Error = CompilerInternalError::NO_ERROR; // Error code if there was any
    std::string ErrorMessage = "";                                 // Error message from parser if there is any

    struct {
        bool Help = false;    // True if --help was passed as a global option
        bool Version = false; // True if --version was passed as a global option
    } GlobalFlags;

    Command Command = Command::NO_COMMAND; // Command to run
    std::vector<std::string> Arguments;    // Arguments
};

} // namespace mirror::utility::cli
