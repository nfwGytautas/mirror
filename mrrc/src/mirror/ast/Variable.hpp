#pragma once

#include <string>

#include "mirror/ast/Type.hpp"

namespace mirror {
namespace ast {

/**
 * @brief Variable class used to encapsulate a variable
 */
class Variable final {
public:
    /**
     * @brief Construct a new Variable object
     *
     * @param type Type of the variable
     * @param name Name of the variable
     */
    Variable(const Type& type, const std::string& name);

private:
    Type m_type;
    std::string m_name;
};

} // namespace ast
} // namespace mirror
