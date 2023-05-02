#include "mirror/lexer/TokenStream.hpp"
#include <sstream>

namespace mirror::lexer {

void TokenStream::add(const TokenStreamEntry& entry) {
    mStream.push_back(entry);
}

bool TokenStream::verify() const {
    return true;
}

const std::string TokenStream::print() const {
    std::stringstream ss;

    static const char* tokenName[] = {"End", "Function",    "->",        "Type annotation", "Identifier",          "==",    "(",
                                      ")",   "Scope begin", "Scope end", "Assign",          "Variable definition", "Return"};

    for (const TokenStreamEntry& entry : mStream) {
        switch (entry.Token) {
        // case Token::Number: {
        //     ss << "Number " << entry.Number;
        //     break;
        // }
        // case Token::String: {
        //     ss << "String " << entry.Identifier;
        //     break;
        // }
        // case Token::Identifier: {
        //     ss << "Identifier " << entry.Identifier;
        //     break;
        // }
        default: {
            ss << tokenName[static_cast<unsigned int>(entry.Token)];
            break;
        }
        }

        ss << "\n";
    }

    return ss.str();
}

} // namespace mirror::lexer
