#include "TokenStream.hpp"
#include <sstream>

namespace mirror {
namespace lexer {

void TokenStream::add(const TokenStreamEntry& entry) {
    m_stream.push_back(entry);
}

bool TokenStream::verify() const {
    return true;
}

TokenQueue TokenStream::getTokenQueue() {
    return TokenQueue(m_stream);
}

const std::string TokenStream::print() const {
    std::stringstream ss;

    static const char* tokenName[] = {"End", "Function",    "->",        "Type annotation", "Identifier",          "==",    "(",
                                      ")",   "Scope begin", "Scope end", "Assign",          "Variable definition", "Return"};

    for (const TokenStreamEntry& entry : m_stream) {
        switch (entry.Token) {
        case Token::Number: {
            ss << "Number " << entry.Number;
            break;
        }
        case Token::String: {
            ss << "String " << entry.Identifier;
            break;
        }
        case Token::Identifier: {
            ss << "Identifier " << entry.Identifier;
            break;
        }
        default: {
            ss << tokenName[static_cast<unsigned int>(entry.Token)];
            break;
        }
        }

        ss << "\n";
    }

    return ss.str();
}

} // namespace lexer
} // namespace mirror
