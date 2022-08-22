#pragma once

#include <vector>

#include "mirror/lexer/TokenStream.hpp"

namespace mirror {
namespace lexer {

class TokenStream::TokenStreamEntry;

/**
 * @brief Simple wrapper to create a token queue from token stream
 */
class TokenQueue final {
public:
    /**
     * @brief Construct a new Token Queue object from a token stream
     *
     * @param vector Token stream
     */
    TokenQueue(std::vector<TokenStream::TokenStreamEntry>& vector);

    /**
     * @brief Pop and return the next stream entry
     *
     * @return TokenStream::TokenStreamEntry object
     */
    TokenStream::TokenStreamEntry pop();

    /**
     * @brief Check the next token in the stream entry
     *
     * @return TokenStream::TokenStreamEntry object
     */
    TokenStream::TokenStreamEntry peek() const;

    /**
     * @brief Ensures the upcoming token is the one specified
     *
     * @param token Token to check
     * @return true Token is the one passed
     * @return false Token is different
     */
    bool ensureNextToken(Token token) const;

    /**
     * @brief Returns the number of entries left
     *
     * @return Number of tokens left
     */
    size_t countLeft() const;

    /**
     * @brief Checks if the queue is empty
     *
     * @return true If the queue is empty
     * @return false Queue still has elements
     */
    bool isEmpty() const;

private:
    size_t m_index = 0;
    std::vector<TokenStream::TokenStreamEntry>& m_vector;
};

} // namespace lexer
} // namespace mirror
