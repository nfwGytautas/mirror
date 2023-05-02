#pragma once

#include <cstdint>
#include <limits>

namespace mirror::lexer {

/**
 * @brief Tokens that are part of the mirror language
 */
enum class Token : uint8_t {
    // Invalid token
    Error = std::numeric_limits<uint8_t>::max(),

    // Empty token returned from tokenizers when they didn't do anything
    None = 0,

    // End of file token representing the end of the translation unit
    End,

    // Token for the 'fn' keyword
    Fn,

    // Token for '->'
    RArrow,

    // Token for type annotation ':'
    TypeAnnotation,

    // Token for text without '"'
    Identifier,

    // Token for '=='
    Equal,

    // Token for '('
    ParenthesisOpen,

    // Token for ')'
    ParenthesisClose,

    // Token for '{'
    ScopeOpen,

    // Token for '}'
    ScopeClose,

    // Token for '='
    Assign,

    // Token for 'var'
    VariableDefinition,

    // Token for 'return'
    Return,

    // Number token
    Number,

    // String token text that starts with '"' and read until another '"'
    String,
};

} // namespace mirror::lexer
