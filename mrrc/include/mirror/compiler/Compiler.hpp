#pragma once

#include "mirror/lexer/TokenStream.hpp"

namespace mirror::compiler {

/**
 * @brief Mirror compiler object
 */
class MRRC final {
public:
    MRRC() = delete;

    /**
     * @brief Compile mirror arguments from command line arguments
     *
     * @param argc Number of arguments
     * @param argv Array of arguments
     * @return int Exit code for more info look at mirror::CompilerInternalError enum
     */
    static int compileArgs(int argc, char** argv);

    /**
     * @brief Run the mirror tokenizer on a file
     *
     * @param file File to tokenize
     * @return TokenStream object
     */
    static lexer::TokenStream tokenizeFile(const char* file);
};

} // namespace mirror::compiler
