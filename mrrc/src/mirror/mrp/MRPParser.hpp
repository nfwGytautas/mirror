#pragma once

#include <filesystem>

#include "mirror/mrp/Project.hpp"

namespace mirror {
namespace mrp {

/**
 * @brief Class used to parse .mrp files
 */
class MRPParser final {
public:
    /**
     * @brief Construct a new MRPParser object from .mrp path
     *
     * @param path Path to .mrp file
     */
    MRPParser(const char* path);

    /**
     * @brief Parse .mrp file, information is stored in the MRPParser
     *
     * @return Returns the Project class
     */
    Project parseProjectFile();

private:
    std::filesystem::path m_projectFile = "";
};

} // namespace mrp
} // namespace mirror
