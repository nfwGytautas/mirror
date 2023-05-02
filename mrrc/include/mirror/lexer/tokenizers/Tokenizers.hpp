#pragma once

#include "CommentTokenizer.hpp"
#include "Tokenizer.hpp"

namespace mirror::lexer::tokenizers {

/**
 * @brief Class that holds all the tokenizers of mirror
 */
class Tokenizers final {
public:
    /**
     * @brief Perform a single pass through all tokenizers
     *
     * @param walker String walker instance
     * @return Parsed token
     */
    static Token run(StringWalker& walker);

    /**
     * @brief Enable trace logging for tokenizers
     */
    static void enableTrace();

    Tokenizers() = delete;
};

} // namespace mirror::lexer::tokenizers
