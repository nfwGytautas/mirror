#include "MRPParser.hpp"
#include <fstream>

#include "mirror/utility/FileParser.hpp"
#include "mirror/utility/Log.hpp"

namespace mirror {
namespace mrp {

MRPParser::MRPParser(const char* path) : m_projectFile(std::filesystem::path(path)) {}

Project MRPParser::parseProjectFile() {
    Project p;

    // Check if .mrp file exists
    if (!std::filesystem::exists(m_projectFile)) {
        LOG_ERROR("File %s doesn't exist", m_projectFile.string().c_str());
        return p;
    }

    file::FileParser parser(m_projectFile);
    std::string word;

    auto verifyAssign = [&]() {
        parser.skipUntilNotEmpty();

        // Make sure '=' is found
        if (parser.getChar() != '=') {
            LOG_ERROR("Expected = after %s", word.c_str());
            return false;
        }

        // Skip until text
        parser.skipUntilNotEmpty();
        return true;
    };

    while (!parser.isFinished()) {
        parser.skipUntilNotEmpty();
        word = parser.readUntil(" =\n");

        if (!verifyAssign()) {
            return p;
        }

        if (word == "PROJECT") {
            // Read until end of line
            p.m_projectName = parser.readUntilEndOfLine();
        } else if (word == "OUTPUT_TOKENS") {
            std::string value = parser.readUntilEndOfLine();

            if (value == "True") {
                p.m_outputTokens = true;
            } else if (value == "False") {
                p.m_outputTokens = false;
            } else {
                LOG_ERROR("Unknown value %s expected True or False", word.c_str());
            }
        } else if (word == "BUILD_DIRECTORY") {
            p.m_buildDirectory = m_projectFile.parent_path() / std::filesystem::path(parser.readUntilEndOfLine());
        } else if (word == "FILES") {
            // Files array
            char sanity = parser.getChar();
            if (sanity != '{') {
                LOG_ERROR("Expected { after FILES keyword got %c", sanity);
                return p;
            }

            // Files are separated by ','
            parser.skipUntilNotEmpty();

            while (parser.peekChar() != '}') {
                std::string file = parser.readUntil(",\n");
                file = (m_projectFile.parent_path() / file).string();
                p.m_units.push_back(mrp::TranslationUnit(file));
                parser.getChar(); // Skip ,
                parser.skipUntilNotEmpty();
            }

            parser.getChar(); // Consume '}'
        } else if (word == "OUTPUT") {
            p.m_output = std::filesystem::path(parser.readUntilEndOfLine());
        } else {
            LOG_ERROR("Unknown value %s", word.c_str());
            return p;
        }
    }

    p.m_isValid = true;
    return p;
}

} // namespace mrp
} // namespace mirror
