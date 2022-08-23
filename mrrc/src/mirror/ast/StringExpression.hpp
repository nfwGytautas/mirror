#pragma once

#include <string>

#include "mirror/ast/BaseExpression.hpp"

namespace mirror {
namespace ast {

/**
 * @brief String constant expression
 */
class StringExpression final : public BaseExpression {
public:
    /**
     * @brief Construct a new String Expression object
     *
     * @param value String value
     */
    StringExpression(const std::string& value);

private:
    std::string m_value;
};

} // namespace ast
} // namespace mirror
