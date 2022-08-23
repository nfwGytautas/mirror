#include "FunctionExpression.hpp"

namespace mirror {
namespace ast {

FunctionExpression::FunctionExpression(const std::string& name) : m_name(name) {}

void FunctionExpression::setReturnType(const Type& type) {
    m_returnType = type;
}

void FunctionExpression::addArgument(const Variable& variable) {
    m_arguments.push_back(variable);
}

void FunctionExpression::setBody(std::unique_ptr<BaseExpression> body) {
    m_body = std::move(body);
}

} // namespace ast
} // namespace mirror
