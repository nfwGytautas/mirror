#pragma once

namespace mirror {
namespace ast {

/**
 * @brief Base Type class
 */
class Type {
public:
    virtual ~Type() {}
};

/**
 * @brief Void type that is empty
 */
class VoidType : public Type {
};

} // namespace ast
} // namespace mirror
