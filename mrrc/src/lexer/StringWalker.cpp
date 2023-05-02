#include "mirror/lexer/StringWalker.hpp"

namespace mirror::lexer {

StringWalker::StringWalker(std::string&& content) : mString(std::move(content)) {}

char StringWalker::getCurrentChar() const {
    return mString[mIndex];
}

void StringWalker::consume() {
    mIndex++;
}

char StringWalker::peekNextChar() const {
    return mString[mIndex + 1];
}

void StringWalker::skipUntilEndOfLine() {
    while (getCurrentChar() != '\n') {
        consume();
    }
    consume(); // Consume the '\n'
}

std::string StringWalker::readUntilEndOfLine() {
    size_t startIdx = mIndex;
    while (getCurrentChar() != '\n') {
        consume();
    }
    consume(); // Consume the '\n'
    return mString.substr(startIdx, mIndex - startIdx);
}

void StringWalker::skipEmpty() {
    while (isCharEmpty(getCurrentChar())) {
        consume();
    }
}

bool StringWalker::isFinished() const {
    return mString.length() == mIndex + 1;
}

bool StringWalker::isCharEmpty(const char c) const {
    return c == ' ' || c == '\t';
}

} // namespace mirror::lexer
