#include "mirror/compiler/Compiler.hpp"

/**
 * @brief If lib mode we exclude mirror compiler main entry and instead expose specific library functions
 */
#ifndef MRRC_LIB_MODE

int main(int argc, char** argv) {
    return mirror::compiler::MRRC::compileArgs(argc, argv);
}

#endif
