#pragma once

#include <string>

#include "mirror/lexer/StringWalker.hpp"
#include "mirror/lexer/TokenStream.hpp"

namespace mirror::lexer {

/**
 * @brief Lexer of mirror
 */
class Lexer final {
public:
    /**
     * @brief Construct a new Lexer object from string input
     *
     * @param rawInput String input
     */
    Lexer(std::string&& rawInput);

    /**
     * @brief Construct a new Lexer object from a file
     *
     * @param file File to read input from
     */
    Lexer(const char* file);

    /**
     * @brief Lex the input and return a token stream
     *
     * @return TokenStream object
     */
    TokenStream lex();

private:
    /**
     * @brief Get the next token in the stream
     *
     * @return Token instance
     */
    Token getNextToken();

private:
    StringWalker mWalker;
    bool mInCommentBlock = false;

    char mCurrentChar = ' ';
    double mNumber;
    std::string mIdentifier = "";
};

} // namespace mirror::lexer
