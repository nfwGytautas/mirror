#include "mirror/lexer/Lexer.hpp"
#include <fstream>
#include <iostream>

#include "mirror/lexer/tokenizers/Tokenizers.hpp"

namespace mirror::lexer {

static bool gVerbose = true;

#define VERBOSE_MSG(x)  \
    if (gVerbose) {     \
        std::cout << x; \
    }

Lexer::Lexer(std::string&& rawInput) : mWalker(std::move(rawInput)) {}

Lexer::Lexer(const char* file) : mWalker("") {
    std::ifstream ifs(file);
    mWalker = StringWalker(std::move(std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()))));
}

TokenStream Lexer::lex() {
    TokenStream result;

    Token t = Token::None;
    while (t != mirror::lexer::Token::End) {
        t = tokenizers::Tokenizers::run(mWalker);

        if (t == Token::Error) {
            // Error
            // TODO: Logging
            std::cout << "Error encountered when tokenizing file" << std::endl;
            abort();
        }

        result.add({t, mIdentifier, mNumber});
    }
    result.add({t, mIdentifier, mNumber});

    return result;
}

// Token Lexer::getNextToken() {
//     mIdentifier = "";
//     mNumber = 0.0;

//     // Remove tabs, spaces, etc.
//     mCurrentChar = mWalker.skipUntilNotEmpty();

//     // Parser finished no more
//     if (mWalker.isFinished()) {
//         return Token::End;
//     }

//     // Get the next character
//     char nextChar = mWalker.peekChar();

//     if (mCurrentChar == '-') {
//         // Right arrow
//         if (mWalker.peekChar() == '>') {
//             mWalker.getChar();
//             mWalker.getChar();
//             return Token::RArrow;
//         }
//     }

//     // Type annotation
//     if (mCurrentChar == ':') {
//         mCurrentChar = mWalker.getChar();
//         return Token::TypeAnnotation;
//     }

//     // String
//     if (mCurrentChar == '"') {
//         // Reset current identifier string
//         mIdentifier = mWalker.readUntil("\"");
//         mWalker.getChar(); // Consume closing '"'
//         return Token::String;
//     }

//     // Identifier
//     if (mCurrentChar == '_' || isalpha(mCurrentChar)) {
//         mIdentifier = mWalker.readUntilSpace();

//         if (mIdentifier == "fn") {
//             mIdentifier = "";
//             return Token::Fn;
//         }

//         if (mIdentifier == "var") {
//             mIdentifier = "";
//             return Token::VariableDefinition;
//         }

//         if (mIdentifier == "return") {
//             mIdentifier = "";
//             return Token::Return;
//         }

//         return Token::Identifier;
//     }

//     // Number
//     if (isdigit(mCurrentChar)) {
//         std::string numberText = mCurrentChar + mWalker.readNumber();
//         mNumber = strtod(numberText.c_str(), 0);
//         return Token::Number;
//     }

//     // Equal
//     if (mCurrentChar == '=') {
//         if (mWalker.peekChar() == '=') {
//             mWalker.getChar();
//             return Token::Equal;
//         }

//         return Token::Assign;
//     }

//     // Parenthesis
//     if (mCurrentChar == '(') {
//         mWalker.getChar(); // Consume
//         return Token::ParenthesisOpen;
//     }

//     if (mCurrentChar == ')') {
//         mWalker.getChar(); // Consume
//         return Token::ParenthesisClose;
//     }

//     // Scope
//     if (mCurrentChar == '{') {
//         mWalker.getChar(); // Consume
//         return Token::ScopeOpen;
//     }

//     if (mCurrentChar == '}') {
//         mWalker.getChar(); // Consume
//         return Token::ScopeClose;
//     }

//     return Token::Error;
// }

} // namespace mirror::lexer
