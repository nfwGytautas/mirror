#pragma once

#include "mirror/lexer/tokenizers/Tokenizer.hpp"

namespace mirror::lexer::tokenizers {

/**
 * @brief Tokenizer that strips away comments
 */
class CommentTokenizer : public Tokenizer {
public:
    /**
     * @brief Perform tokenization and return the token
     *
     * @param walker String walker
     * @return Token Token to tokenize
     */
    Token tokenize(StringWalker& walker) override;

private:
    /**
     * @brief Keep walking until the end of the comment block is found
     *
     * @param walker String walker
     */
    void skipUntilBlockEnd(StringWalker& walker);
};

} // namespace mirror::lexer::tokenizers
