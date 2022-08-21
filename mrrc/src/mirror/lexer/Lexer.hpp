#pragma once

#include <string>

#include "mirror/lexer/Token.hpp"
#include "mirror/mrp/TranslationUnit.hpp"
#include "mirror/utility/FileParser.hpp"

namespace mirror {
namespace lexer {

/**
 * @brief Mirror source lexer
 */
class Lexer final {
public:
    Lexer(mrp::TranslationUnit& translationUnit);
    ~Lexer();

    /**
     * @brief Lex the translation unit
     *
     * @return true If lexing completed without errors
     * @return false Lexing failed at any point
     */
    bool lex();

private:
    /**
     * @brief Get the next token from the lexer
     *
     * @return Token type
     */
    Token getToken();

private:
    mrp::TranslationUnit& m_unit;
    file::FileParser m_parser;

    char m_currentChar = ' ';
    double m_number;
    std::string m_identifierString = "";
};

} // namespace lexer
} // namespace mirror
