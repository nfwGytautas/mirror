#pragma once

#include <string>

namespace mirror {
namespace ast {

/**
 * @brief Type class
 */
class Type final {
public:
    /**
     * @brief Construct a new Type object
     *
     * @param name Name of the type
     */
    Type(const std::string& name);

private:
    std::string m_name;
};

} // namespace ast
} // namespace mirror
