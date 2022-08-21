#include "FileParser.hpp"
#include <cctype>

#include "mirror/utility/Log.hpp"

namespace mirror {
namespace file {

FileParser::FileParser(const std::filesystem::path& path) : m_in(path) {
    if (!m_in.good()) {
        LOG_ERROR("Failed to open %s : %s", path.string().c_str());
    }
}

FileParser::~FileParser() {
    m_in.close();
}

bool FileParser::isFinished() {
    return m_in.peek() == EOF;
}

std::string FileParser::readUntil(const std::string& delim) {
    std::string word;
    char c = peekChar();

    while (delim.find(c) == std::string::npos) {
        word += getChar();
        c = peekChar();
    }

    return word;
}

char FileParser::peekChar() {
    return m_in.peek();
}

char FileParser::getChar() {
    m_in.get(m_lastChar);
    return m_lastChar;
}

char FileParser::skipUntilNotEmpty() {
    char c = peekChar();
    while (!(isNotEmpty(c))) {
        getChar();      // Ignore this character
        c = peekChar(); // Check the next character
    }
    return c;
}

std::string FileParser::readUntilEndOfLine() {
    std::string line;
    std::getline(m_in, line);
    return line;
}

std::string FileParser::readNumber() {
    std::string number;
    uint8_t sepCount = 0;

    do {
        number += getChar();

        if (peekChar() == '.') {
            sepCount += 1;

            if (sepCount > 1) {
                break;
            }
        }
    } while (isdigit(peekChar()) || peekChar() == '.');

    return number;
}

std::string FileParser::readUntilSpace() {
    std::string text;

    char c = peekChar();
    while (isalnum(c) || c == '_') {
        text += getChar();
        c = peekChar();
    }

    return text;
}

bool FileParser::isNotEmpty(const char c) {
    return !(isspace(c));
}

} // namespace file
} // namespace mirror
