#include "mirror/lexer/tokenizers/Tokenizer.hpp"
#include <iostream>

namespace mirror::lexer::tokenizers {

void Tokenizer::enableTrace() {
    mTraceEnabled = true;
}

void Tokenizer::traceMessage(const char* msg) {
    if (mTraceEnabled) {
        std::cout << msg;
    }
}

} // namespace mirror::lexer::tokenizers
