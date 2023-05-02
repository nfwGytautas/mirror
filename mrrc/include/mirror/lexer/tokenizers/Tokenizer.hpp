#pragma once

#include "mirror/lexer/StringWalker.hpp"
#include "mirror/lexer/Token.hpp"

namespace mirror::lexer::tokenizers {

/**
 * @brief Tokenizer base class
 */
class Tokenizer {
public:
    virtual ~Tokenizer() = default;

    /**
     * @brief Perform tokenization and return the token
     *
     * @param walker String walker
     * @return Token Token to tokenize
     */
    virtual Token tokenize(StringWalker& walker) = 0;

    /**
     * @brief Enable tracing of tokenizer actions
     */
    void enableTrace();

protected:
    /**
     * @brief Print a trace message
     *
     * @param msg Message to print
     */
    void traceMessage(const char* msg);

private:
    bool mTraceEnabled = false;
};

} // namespace mirror::lexer::tokenizers
