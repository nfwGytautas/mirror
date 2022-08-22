#include "TokenQueue.hpp"

namespace mirror {
namespace lexer {

TokenQueue::TokenQueue(std::vector<TokenStream::TokenStreamEntry>& vector) : m_vector(vector) {}

TokenStream::TokenStreamEntry TokenQueue::pop() {
    return m_vector[m_index++];
}

TokenStream::TokenStreamEntry TokenQueue::peek() const {
    return m_vector[m_index + 1];
}

bool TokenQueue::ensureNextToken(Token token) const {
    return m_vector[m_index + 1].Token == token;
}

size_t TokenQueue::countLeft() const {
    return m_vector.size() - m_index;
}

bool TokenQueue::isEmpty() const {
    return m_vector.size() == m_index;
}

} // namespace lexer
} // namespace mirror
