#include "utility/cli/CLIParser.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

namespace mirror::utility::cli {

static const std::unordered_map<std::string, GlobalOption> gGlobalOptionsMap = {
    {"--help", GlobalOption::HELP},
    {"-h", GlobalOption::HELP},
    {"--version", GlobalOption::VERSION},
    {"-v", GlobalOption::VERSION},
};

CLIParser::CLIParser(int argc, char** argv) : Empty(argc == 1) {
    bool globalOptionsDone = false;
    bool commandOptions = false;

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
                // Command option?
            }

        } else {
            // Global options disabled
            globalOptionsDone = true;
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

} // namespace mirror::utility::cli
