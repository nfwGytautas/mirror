#pragma once

#include <string>
#include <filesystem>
#include <vector>

#include "mirror/mrp/TranslationUnit.hpp"

namespace mirror {
namespace mrp {

/**
 * @brief MRP project class
 */
class Project final {
public:
    bool m_isValid = false;
    std::string m_projectName = "";
    bool m_outputTokens = false;
    std::filesystem::path m_buildDirectory = "";
    std::vector<TranslationUnit> m_units;
    std::filesystem::path m_output = "";
};

} // namespace mrp
} // namespace mirror
