#pragma once

#include <string>

#include "mirror/ast/BaseExpression.hpp"

namespace mirror {
namespace ast {

/**
 * @brief Variable reference expression
 */
class VariableExpression final : public BaseExpression {
public:
    /**
     * @brief Construct a new Variable Expression object
     *
     * @param name Name of the variable
     */
    VariableExpression(const std::string& name);

private:
    std::string m_name;
};

} // namespace ast
} // namespace mirror
