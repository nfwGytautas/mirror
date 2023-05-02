#include "mirror/lexer/tokenizers/CommentTokenizer.hpp"

namespace mirror::lexer::tokenizers {

Token CommentTokenizer::tokenize(StringWalker& walker) {
    char currentChar = walker.getCurrentChar();
    char nextChar = walker.peekNextChar();

    // Comment
    if (currentChar == '/' && nextChar == '/') {
        // Read comment and ignore it's result
        auto comment = walker.readUntilEndOfLine();
        traceMessage("Single line comment: ");
        traceMessage(comment.c_str());
        traceMessage("\n");
        return Token::None;
    }

    // Comment block begin
    if (currentChar == '/' && nextChar == '*') {
        traceMessage("Comment block begin\n");

        // Read the header of the block comment
        walker.skipUntilEndOfLine();

        // Read until comment block end
        skipUntilBlockEnd(walker);

        return Token::None;
    }

    return Token::None;
}

void CommentTokenizer::skipUntilBlockEnd(StringWalker& walker) {
    walker.skipEmpty(); // Consume empty characters

    char currentChar = walker.getCurrentChar();
    char nextChar = walker.peekNextChar();

    if (currentChar == '*' && nextChar == '/') {
        traceMessage("Comment block end\n");
        walker.skipUntilEndOfLine();
        return;
    }

    auto comment = walker.readUntilEndOfLine();
    traceMessage("Block comment entry: ");
    traceMessage(comment.c_str());

    // Recurse
    skipUntilBlockEnd(walker);
}

} // namespace mirror::lexer::tokenizers
