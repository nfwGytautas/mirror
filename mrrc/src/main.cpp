#include <fstream>

#include "mirror/lexer/Lexer.hpp"
#include "mirror/mrp/MRPParser.hpp"
#include "mirror/utility/Log.hpp"

int main(int argc, char** argv) {
    // Check that there is an argument to .mrp file
    if (argc != 2) {
        LOG_ERROR("Please specify the path to .mrp file")
        return 1;
    }

    // Create mrp parser object
    mirror::mrp::MRPParser mrpParser(argv[1]);

    // Parse the project and verify that it is valid
    mirror::mrp::Project project = mrpParser.parseProjectFile();
    if (!project.m_isValid) {
        return 2;
    }

    for (mirror::mrp::TranslationUnit& unit : project.m_units) {
        LOG_INFO("Processing %s", unit.getFile().string().c_str());

        // Lex the project
        mirror::lexer::Lexer lexer(unit);
        if (!lexer.lex()) {
            LOG_ERROR("Failed to lex %s", unit.getFile().string());
            continue;
        }

        // Verify the token stream
        if (!unit.getTokenStream().verify()) {
            LOG_ERROR("Failed to verify token stream %s", unit.getFile().string());
            continue;
        }

        // If enabled output the token stream to file
        if (project.m_outputTokens) {
            if (!std::filesystem::exists(project.m_buildDirectory)) {
                std::filesystem::create_directory(project.m_buildDirectory);
            }

            const std::filesystem::path outPath = (project.m_buildDirectory / (unit.getFile().stem().string() + ".mts"));
            std::ofstream out(outPath);
            out << unit.getTokenStream().print();
        }

        // Create the abstract syntax tree


    }

    return 0;
}
