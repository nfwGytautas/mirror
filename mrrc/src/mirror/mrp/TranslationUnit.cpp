#include "TranslationUnit.hpp"

namespace mirror {
namespace mrp {

TranslationUnit::TranslationUnit(const std::filesystem::path& file) : m_file(file) {}

const std::filesystem::path& TranslationUnit::getFile() const {
    return m_file;
}

void TranslationUnit::setTokenStream(const lexer::TokenStream& stream) {
    m_tokenStream = stream;
}

lexer::TokenStream& TranslationUnit::getTokenStream() {
    return m_tokenStream;
}

} // namespace mrp
} // namespace mirror
