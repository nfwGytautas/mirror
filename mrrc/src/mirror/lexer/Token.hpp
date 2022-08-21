#pragma once

namespace mirror {
namespace lexer {

/**
 * @brief Enum class representing possible Tokens
 */
enum class Token : unsigned int {
    // Invalid token
    Error = UINT_MAX,

    // End of file token representing the end of the translation unit
    End = 0,

    // Token for the 'fn' keyword
    Fn = 1,

    // Token for '->'
    RArrow = 2,

    // Token for type annotation ':'
    TypeAnnotation = 3,

    // Token for text without '"'
    Identifier = 4,

    // Token for '=='
    Equal = 5,

    // Token for '('
    ParenthesisOpen = 6,

    // Token for ')'
    ParenthesisClose = 7,

    // Token for '{'
    ScopeOpen = 8,

    // Token for '}'
    ScopeClose = 9,

    // Token for '='
    Assign = 10,

    // Token for 'var'
    VariableDefinition = 11,

    // Token for 'return'
    Return = 12,

    // Number token
    Number = 50,

    // String token text that starts with '"' and read until another '"'
    String = 51,
};

} // namespace lexer
} // namespace mirror
