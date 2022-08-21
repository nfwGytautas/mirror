#pragma once

#include <string>
#include <vector>

#include "mirror/lexer/Token.hpp"

namespace mirror {
namespace lexer {

/**
 * @brief Token stream object used to then construct abstract syntax trees
 */
class TokenStream final {
public:
    /**
     * @brief Entry for token stream
     */
    struct TokenStreamEntry {
        Token Token;
        std::string Identifier;
        double Number;
    };

    /**
     * @brief Add entry to token stream
     *
     * @param entry Entry struct
     */
    void add(const TokenStreamEntry& entry);

    /**
     * @brief Verify that the token stream is logical e.g. is structured correctly
     *
     * @return true Is correctly structured and can be assembled into AST
     * @return false Lexed incorrectly syntax error
     */
    bool verify() const;

    /**
     * @brief Prints the token stream to the output console for debugging purposes
     */
    const std::string print() const;

private:
    std::vector<TokenStreamEntry> m_stream;
};

} // namespace lexer
} // namespace mirror
