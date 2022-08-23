#include "Parser.hpp"

#include "mirror/ast/FunctionCallExpression.hpp"
#include "mirror/ast/FunctionExpression.hpp"
#include "mirror/ast/ScopeExpression.hpp"
#include "mirror/ast/StringExpression.hpp"
#include "mirror/ast/VariableExpression.hpp"
#include "mirror/lexer/TokenQueue.hpp"
#include "mirror/lexer/TokenStream.hpp"
#include "mirror/utility/Log.hpp"

namespace mirror {
namespace parser {

Parser::Parser(mrp::TranslationUnit& translationUnit) : m_unit(translationUnit) {}

bool Parser::parse() {
    lexer::TokenQueue queue = m_unit.getTokenStream().getTokenQueue();
    std::vector<std::unique_ptr<ast::BaseExpression>> expressions;

    while (!queue.isEmpty()) {
        std::unique_ptr<ast::BaseExpression> result = parseOne(queue);

        // Check if expression was parsed
        if (!result) {
            return false;
        }

        expressions.push_back(std::move(result));
    }

    return true;
}

std::unique_ptr<ast::BaseExpression> Parser::parseOne(lexer::TokenQueue& tokens) {
    switch (tokens.peek().Token) {
    case lexer::Token::Fn: {
        return parseFunction(tokens);
    }
    case lexer::Token::Identifier: {
        return parseIdentifier(tokens);
    }
    case lexer::Token::String: {
        return parseString(tokens);
    }
    default: {
        LOG_ERROR("Unexpected token %ld", static_cast<unsigned int>(tokens.peek().Token));
        return nullptr;
    }
    }
}

std::unique_ptr<ast::BaseExpression> Parser::parseFunction(lexer::TokenQueue& tokens) {
    // fn name ( args... ) '-> type'?
    tokens.pop(); // Consume fn
    auto name = tokens.pop();

    if (name.Token != lexer::Token::Identifier || name.Identifier.empty()) {
        return nullptr;
    }

    auto function = std::make_unique<ast::FunctionExpression>(name.Identifier);

    auto openParenthesis = tokens.pop();
    if (openParenthesis.Token != lexer::Token::ParenthesisOpen) {
        return nullptr;
    }

    // Parse arguments
    while (tokens.peek().Token != lexer::Token::ParenthesisClose) {
        // name : type
        auto argName = tokens.pop();

        if (argName.Token != lexer::Token::Identifier || argName.Identifier.empty()) {
            return nullptr;
        }

        if (tokens.peek().Token != lexer::Token::TypeAnnotation) {
            return nullptr;
        }
        tokens.pop(); // Consume :

        auto type = tokens.pop();

        if (type.Token != lexer::Token::Identifier || type.Identifier.empty()) {
            return nullptr;
        }

        function->addArgument(ast::Variable(type.Identifier, argName.Identifier));
    }

    tokens.pop(); // Consume )

    // Check if has return type
    if (tokens.peek().Token == lexer::Token::RArrow) {
        // Return type
        tokens.pop(); // Consume ->
        auto returnType = tokens.pop();

        function->setReturnType(ast::Type(returnType.Identifier));
    } else {
        function->setReturnType(ast::Type("void"));
    }

    if (tokens.peek().Token != lexer::Token::ScopeOpen) {
        return nullptr;
    }

    function->setBody(parseScope(tokens));

    return std::move(function);
}

std::unique_ptr<ast::BaseExpression> Parser::parseScope(lexer::TokenQueue& tokens) {
    // { Expression... }
    auto scope = std::make_unique<ast::ScopeExpression>();

    tokens.pop(); // Consume {

    while (tokens.peek().Token != lexer::Token::ParenthesisClose) {
        std::unique_ptr<ast::BaseExpression> result = parseOne(tokens);
        scope->addExpression(std::move(result));
    }

    tokens.pop(); // Consume }
    return std::move(scope);
}

std::unique_ptr<ast::BaseExpression> Parser::parseIdentifier(lexer::TokenQueue& tokens) {
    // identifier
    // identifier ( args... )

    auto identifier = tokens.pop();

    if (tokens.peek().Token != lexer::Token::ParenthesisOpen) {
        return std::make_unique<ast::VariableExpression>(identifier.Identifier);
    }

    tokens.pop(); // Consume (

    std::unique_ptr<ast::FunctionCallExpression> fnCall = std::make_unique<ast::FunctionCallExpression>(identifier.Identifier);

    while (tokens.peek().Token != lexer::Token::ParenthesisClose) {
        auto expression = parseOne(tokens);

        if (!expression) {
            return nullptr;
        }

        fnCall->addArgument(std::move(expression));
    }

    tokens.pop(); // Consume )

    return std::move(fnCall);
}

std::unique_ptr<ast::BaseExpression> Parser::parseString(lexer::TokenQueue& tokens) {
    // string
    return std::make_unique<ast::StringExpression>(tokens.pop().Identifier);
}

} // namespace parser
} // namespace mirror
