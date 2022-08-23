#include "ScopeExpression.hpp"

namespace mirror {
namespace ast {

void ScopeExpression::addExpression(std::unique_ptr<BaseExpression> expression) {
    m_body.push_back(std::move(expression));
}

} // namespace ast
} // namespace mirror
