#pragma once

#include <filesystem>

#include "mirror/lexer/TokenStream.hpp"

namespace mirror {
namespace mrp {

/**
 * @brief A translation unit class used to encapsulate a single translation unit (mirror file)
 */
class TranslationUnit final {
public:
    /**
     * @brief Construct a new Translation Unit object
     *
     * @param file File of the translation unit
     */
    TranslationUnit(const std::filesystem::path& file);

    /**
     * @brief Get the file of the translation unit
     *
     * @return Path to the translation unit file
     */
    const std::filesystem::path& getFile() const;

    /**
     * @brief Set the token stream of the translation unit
     *
     * @param stream Stream to set
     */
    void setTokenStream(const lexer::TokenStream& stream);

    /**
     * @brief Returns the token stream of the translation unit
     *
     * @return Token stream object
     */
    const lexer::TokenStream& getTokenStream() const;

private:
    std::filesystem::path m_file;
    lexer::TokenStream m_tokenStream;
};

} // namespace mrp
} // namespace mirror
