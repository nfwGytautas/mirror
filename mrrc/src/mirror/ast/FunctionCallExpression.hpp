#pragma once

#include <memory>
#include <string>
#include <vector>

#include "mirror/ast/BaseExpression.hpp"

namespace mirror {
namespace ast {

/**
 * @brief A function call expression
 */
class FunctionCallExpression final : public BaseExpression {
public:
    /**
     * @brief Construct a new Function Call Expression object
     *
     * @param fn Function name
     */
    FunctionCallExpression(const std::string& fn);

    /**
     * @brief Add argument to the function call
     *
     * @param argument Argument to add
     */
    void addArgument(std::unique_ptr<BaseExpression> argument);

private:
    std::string m_function;
    std::vector<std::unique_ptr<BaseExpression>> m_args;
};

} // namespace ast
} // namespace mirror
