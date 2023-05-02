#include "mirror/lexer/tokenizers/Tokenizers.hpp"

namespace mirror::lexer::tokenizers {

static const std::unique_ptr<Tokenizer> gTokenizers[] = {std::make_unique<CommentTokenizer>()};

static constexpr size_t gNumTokenizers = sizeof(gTokenizers) / sizeof(gTokenizers[0]);

Token Tokenizers::run(StringWalker& walker) {
    walker.skipEmpty(); // Skip empty

    if (walker.isFinished()) {
        return Token::End;
    }

    for (size_t i = 0; i < gNumTokenizers; i++) {
        Token result = gTokenizers[i]->tokenize(walker);
        if (result != Token::None) {
            return result;
        }
    }
    return Token::Error;
}

void Tokenizers::enableTrace() {
    for (size_t i = 0; i < gNumTokenizers; i++) {
        gTokenizers[i]->enableTrace();
    }
}

} // namespace mirror::lexer::tokenizers
