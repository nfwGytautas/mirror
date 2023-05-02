#include "utility/cli/CLIParser.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

#include "mirror/lexer/tokenizers/Tokenizers.hpp"

namespace mirror::utility::cli {

static const std::unordered_map<std::string, GlobalOption> gGlobalOptionsMap = {
    {"--help", GlobalOption::HELP},
    {"-h", GlobalOption::HELP},
    {"--version", GlobalOption::VERSION},
    {"-v", GlobalOption::VERSION},
};

static const std::unordered_map<std::string, TokensOptions> gTokensOptionsMap = {
    {"--trace", TokensOptions::TRACE},
    {"-t", TokensOptions::TRACE},
};

static const std::unordered_map<std::string, Command> gCommandMap = {
    {"tokens", Command::TOKENS},
};

CLIParser::CLIParser(int argc, char** argv) : Empty(argc == 1) {
    bool globalOptionsDone = false;

    for (int i = 1; i < argc; i++) {
        const std::string arg = argv[i];

        if (arg[0] == '-') {
            const auto globalOptionIt = gGlobalOptionsMap.find(arg);
            if (globalOptionIt != gGlobalOptionsMap.end()) {
                // Global option
                if (globalOptionsDone) {
                    // Not allowed
                    Error = CompilerInternalError::BAD_ARGUMENTS;
                    ErrorMessage = "Option " + arg + " is misplaced (global options have to go after mrrc command)";
                    return;
                }

                reactToGlobalOption(globalOptionIt->second, arg.c_str());
            } else {
                reactToCommandOption(arg);

                // Command option?
                if (Arguments.size() != 0) {
                    // Tried to specify option in the middle of a argument
                    Error = CompilerInternalError::BAD_ARGUMENTS;
                    ErrorMessage = "Option " + arg + " is misplaced (command or global options can't appear in the argument list)";
                    return;
                }
            }

        } else {
            // Global options disabled
            globalOptionsDone = true;

            if (Command == Command::NO_COMMAND) {
                const auto commandIt = gCommandMap.find(arg);
                if (commandIt != gCommandMap.end()) {
                    Command = commandIt->second;
                }
            } else {
                // Arguments
                Arguments.push_back(arg);
            }
        }
    }
}

void CLIParser::reactToGlobalOption(GlobalOption option, const char* optionString) {
    switch (option) {
    case GlobalOption::HELP:
        GlobalFlags.Help = true;
        break;

    case GlobalOption::VERSION:
        GlobalFlags.Version = true;
        break;

    default:
        // Unknown option
        Error = CompilerInternalError::BAD_ARGUMENTS;
        ErrorMessage = "Unknown option " + std::string(optionString);
        break;
    }
}

void CLIParser::reactToCommandOption(const std::string& option) {
    switch (Command) {
    case Command::TOKENS: {
        auto tokensOption = gTokensOptionsMap.find(option);
        if (tokensOption == gTokensOptionsMap.end()) {
            Error = CompilerInternalError::BAD_ARGUMENTS;
            ErrorMessage = "Tokens command doesn't recognize option " + option;
            return;
        }

        switch (tokensOption->second) {
        case TokensOptions::TRACE: {
            mirror::lexer::tokenizers::Tokenizers::enableTrace();
            break;
        }

        default: {
            Error = CompilerInternalError::BAD_ARGUMENTS;
            ErrorMessage = "Tokens command doesn't recognize option " + option;
            break;
        }
        }

        break;
    }
    default:
        // Command has no options
        Error = CompilerInternalError::BAD_ARGUMENTS;
        ErrorMessage = "Command " + option + " takes no options";
        break;
    }
}

} // namespace mirror::utility::cli
