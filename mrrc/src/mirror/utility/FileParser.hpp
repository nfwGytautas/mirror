#pragma once

#include <filesystem>
#include <fstream>

namespace mirror {
namespace file {

/**
 * @brief Utility class for parsing files
 */
class FileParser final {
public:
    /**
     * @brief Construct a new File Parser object
     *
     * @param file File to parse
     */
    FileParser(const std::filesystem::path& file);
    ~FileParser();

    /**
     * @brief Returns true if the file has been completely iterated through, false otherwise
     */
    bool isFinished();

    /**
     * @brief Returns text from the file until the specified delimiter is found
     *
     * @param delim Delimeter
     * @return Read text
     */
    std::string readUntil(const std::string& delim = " ");

    /**
     * @brief Return the next character in file without moving the stream position
     *
     * @return char Peeked character
     */
    char peekChar();

    /**
     * @brief Return the next character in the file
     *
     * @return char Character read from file
     */
    char getChar();

    /**
     * @brief Moves the stream pointer until it points to anything that isn't:
     *  - Whitespace
     *  - End of line '\n'
     *  - Tab
     *
     * @return Next character after empty
     */
    char skipUntilNotEmpty();

    /**
     * @brief Read the rest of the line and return the content
     *
     * @return Text from current position to the next end of the line
     */
    std::string readUntilEndOfLine();

    /**
     * @brief Read a number and return it
     *
     * @return Number text
     */
    std::string readNumber();

    /**
     * @brief Read until a space and return it
     *
     * @return Read text
     */
    std::string readUntilSpace();

private:
    /**
     * @brief Checks if the passed character is empty or not
     *
     * @param c character to check
     * @return true if the character is not empty, false otherwise
     */
    static bool isNotEmpty(const char c);

private:
    std::ifstream m_in;
    char m_lastChar = '0';
};
} // namespace file
} // namespace mirror
