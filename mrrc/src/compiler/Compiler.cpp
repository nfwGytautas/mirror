#include "mirror/compiler/Compiler.hpp"
#include <iostream>

#include "mirror/Errors.hpp"
#include "mirror/lexer/Lexer.hpp"

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

    // Commands
    if (cli.Command == utility::cli::Command::TOKENS) {
        // Output tokens
        std::cout << "Outputting tokens\n";
        for (const std::string& inputFile : cli.Arguments) {
            std::cout << tokenizeFile(inputFile.c_str()).print() << std::endl;
        }
    }

    return static_cast<int>(CompilerInternalError::NO_ERROR);
}

lexer::TokenStream MRRC::tokenizeFile(const char* file) {
    lexer::Lexer lexer(file);
    return lexer.lex();
}

} // namespace mirror::compiler
