#pragma once

#include <memory>
#include <queue>

#include "mirror/ast/BaseExpression.hpp"
#include "mirror/mrp/TranslationUnit.hpp"

namespace mirror {
namespace parser {

/**
 * @brief Parser used to construct AST from token stream
 */
class Parser final {
public:
    /**
     * @brief Construct a new Parser object for the translation unit
     *
     * @param translationUnit Unit this parser will parse
     */
    Parser(mrp::TranslationUnit& translationUnit);

    /**
     * @brief Parse the translation unit
     *
     * @return true If parsing completed without errors
     * @return false Parsing failed at any point
     */
    bool parse();

private:
    /**
     * @brief Parse a single AST
     *
     * @param tokens Token queue
     * @return std::unique_ptr<ast::BaseExpression> instance
     */
    std::unique_ptr<ast::BaseExpression> parseOne(lexer::TokenQueue& tokens);

    /**
     * @brief Parse a function AST
     *
     * @param tokens Token queue
     * @return std::unique_ptr<ast::BaseExpression> instance
     */
    std::unique_ptr<ast::BaseExpression> parseFunction(lexer::TokenQueue& tokens);

    /**
     * @brief Parse a scope AST
     *
     * @param tokens Token queue
     * @return std::unique_ptr<ast::BaseExpression> instance
     */
    std::unique_ptr<ast::BaseExpression> parseScope(lexer::TokenQueue& tokens);

    /**
     * @brief Parse a identifier AST
     *
     * @param tokens Token queue
     * @return std::unique_ptr<ast::BaseExpression> instance
     */
    std::unique_ptr<ast::BaseExpression> parseIdentifier(lexer::TokenQueue& tokens);

    /**
     * @brief Parse a string AST
     *
     * @param tokens Token queue
     * @return std::unique_ptr<ast::BaseExpression> instance
     */
    std::unique_ptr<ast::BaseExpression> parseString(lexer::TokenQueue& tokens);

private:
    mrp::TranslationUnit& m_unit;
    size_t m_tokenIndex = 0;
};

} // namespace parser
} // namespace mirror
