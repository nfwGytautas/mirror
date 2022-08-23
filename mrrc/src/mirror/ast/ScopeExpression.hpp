#pragma once

#include <memory>
#include <vector>

#include "mirror/ast/BaseExpression.hpp"

namespace mirror {
namespace ast {

/**
 * @brief Scope expression class
 */
class ScopeExpression final : public BaseExpression {
public:
    /**
     * @brief Add argument to function
     *
     * @param variable Variable type
     */
    void addExpression(std::unique_ptr<BaseExpression> expression);

private:
    std::vector<std::unique_ptr<BaseExpression>> m_body;
};

} // namespace ast
} // namespace mirror
