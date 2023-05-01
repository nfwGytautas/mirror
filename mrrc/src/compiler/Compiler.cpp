#include "mirror/compiler/Compiler.hpp"
#include <iostream>

#include "mirror/Errors.hpp"

#include "utility/HelpMessage.hpp"
#include "utility/cli/CLIParser.hpp"

namespace mirror::compiler {

int MRRC::compileArgs(int argc, char** argv) {
    utility::cli::CLIParser cli(argc, argv);

    if (cli.Error != CompilerInternalError::NO_ERROR) {
        // There was an error parsing
        std::cout << cli.ErrorMessage;
        return static_cast<int>(cli.Error);
    }

    if (cli.Empty || cli.GlobalFlags.Help) {
        // No arguments given print help
        std::cout << gHelpMessage;
        return static_cast<int>(CompilerInternalError::NO_ERROR);
    }

    if (cli.GlobalFlags.Version) {
        std::cout << "Mirror compiler v" << MRRC_VERSION << "\n";
    }

    return static_cast<int>(CompilerInternalError::NO_ERROR);
}

} // namespace mirror::compiler
