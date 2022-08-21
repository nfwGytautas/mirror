#pragma once

#include <string>
#include <vector>

#include "mirror/ast/BaseExpression.hpp"
#include "mirror/ast/Type.hpp"
#include "mirror/ast/Variable.hpp"

namespace mirror {
namespace ast {

/**
 * @brief Function expression class
 */
class FunctionExpression : public BaseExpression {
public:
    /**
     * @brief Construct a new Function Expression object
     *
     * @param name Name of the function
     */
    FunctionExpression(const std::string& name);

    /**
     * @brief Set the return type of this function
     *
     * @param type Type of the return
     */
    void setReturnType(const Type& type);

    /**
     * @brief Add argument to function
     *
     * @param variable Variable type
     */
    void addArgument(const Variable& variable);

private:
    std::string m_name = "";
    Type m_returnType = VoidType();
    std::vector<Variable> m_arguments;
};

} // namespace ast
} // namespace mirror
