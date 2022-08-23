#include "FunctionCallExpression.hpp"

namespace mirror {
namespace ast {

FunctionCallExpression::FunctionCallExpression(const std::string& fn) : m_function(fn) {}

void FunctionCallExpression::addArgument(std::unique_ptr<BaseExpression> argument) {
    m_args.push_back(std::move(argument));
}

} // namespace ast
} // namespace mirror
