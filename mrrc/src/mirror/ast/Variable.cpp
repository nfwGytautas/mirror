#include "Variable.hpp"

namespace mirror {
namespace ast {

Variable::Variable(const Type& type, const std::string& name) : m_type(type), m_name(name) {}

} // namespace ast
} // namespace mirror
