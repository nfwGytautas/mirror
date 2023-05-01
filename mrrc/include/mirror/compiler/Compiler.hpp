#pragma once

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
};

} // namespace mirror::compiler
