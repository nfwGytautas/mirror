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
class FunctionExpression final : public BaseExpression {
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

    /**
     * @brief Set the body of the function
     *
     * @param body Body of the function
     */
    void setBody(std::unique_ptr<BaseExpression> body);

private:
    std::string m_name = "";
    Type m_returnType = Type("void");
    std::vector<Variable> m_arguments;
    std::unique_ptr<BaseExpression> m_body;
};

} // namespace ast
} // namespace mirror
