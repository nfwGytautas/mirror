#include "Lexer.hpp"

#include "mirror/utility/FileParser.hpp"
#include "mirror/utility/Log.hpp"

namespace mirror {
namespace lexer {

Lexer::Lexer(mrp::TranslationUnit& translationUnit) : m_unit(translationUnit), m_parser(translationUnit.getFile()) {}

Lexer::~Lexer() {}

bool Lexer::lex() {
    TokenStream stream;

    Token t = getToken();
    while (t != mirror::lexer::Token::End) {
        stream.add({t, m_identifierString, m_number});
        t = getToken();
    }
    stream.add({t, m_identifierString, m_number});

    m_unit.setTokenStream(stream);
    return true;
}

Token Lexer::getToken() {
    m_identifierString = "";
    m_number = 0.0;

    // Remove tabs, spaces, etc.
    m_parser.skipUntilNotEmpty();

    // Parser finished no more
    if (m_parser.isFinished()) {
        return Token::End;
    }

    // Get the char
    m_currentChar = m_parser.getChar();

    // Comment
    if (m_currentChar == '/' && m_parser.peekChar() == '/') {
        // Read comment and ignore it's result
        m_parser.readUntilEndOfLine();

        // Repeat the getToken() but on new line
        return getToken();
    }

    if (m_currentChar == '-') {
        // Right arrow
        if (m_parser.peekChar() == '>') {
            m_parser.getChar();
            return Token::RArrow;
        }
    }

    // Type annotation
    if (m_currentChar == ':') {
        m_currentChar = m_parser.getChar();
        return Token::TypeAnnotation;
    }

    // String
    if (m_currentChar == '"') {
        // Reset current identifier string
        m_identifierString = m_parser.readUntil("\"");
        m_parser.getChar(); // Consume closing '"'
        return Token::String;
    }

    // Identifier
    if (m_currentChar == '_' || isalpha(m_currentChar)) {
        m_identifierString = m_currentChar + m_parser.readUntilSpace();

        if (m_identifierString == "fn") {
            m_identifierString = "";
            return Token::Fn;
        }

        if (m_identifierString == "var") {
            m_identifierString = "";
            return Token::VariableDefinition;
        }

        if (m_identifierString == "return") {
            m_identifierString = "";
            return Token::Return;
        }

        return Token::Identifier;
    }

    // Number
    if (isdigit(m_currentChar)) {
        std::string numberText = m_currentChar + m_parser.readNumber();
        m_number = strtod(numberText.c_str(), 0);
        return Token::Number;
    }

    // Equal
    if (m_currentChar == '=') {
        if (m_parser.peekChar() == '=') {
            m_parser.getChar();
            return Token::Equal;
        }

        return Token::Assign;
    }

    // Parenthesis
    if (m_currentChar == '(') {
        return Token::ParenthesisOpen;
    }

    if (m_currentChar == ')') {
        return Token::ParenthesisClose;
    }

    // Scope
    if (m_currentChar == '{') {
        return Token::ScopeOpen;
    }

    if (m_currentChar == '}') {
        return Token::ScopeClose;
    }

    LOG_ERROR("Failed while lexing %s", m_currentChar);
    return Token::Error;
}

} // namespace lexer
} // namespace mirror
