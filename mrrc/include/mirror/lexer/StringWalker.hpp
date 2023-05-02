#pragma once

#include <string>

namespace mirror::lexer {

/**
 * @brief String walker class that has utility functions for navigating a string
 */
class StringWalker final {
public:
    /**
     * @brief Construct a new String Walker object
     *
     * @param content String to walk
     */
    StringWalker(std::string&& content);

    /**
     * @brief Get the current character at the walker index
     *
     * @return current character
     */
    char getCurrentChar() const;

    /**
     * @brief Consume the current character (advances the index by 1)
     */
    void consume();

    /**
     * @brief Get the character at the next walker index
     *
     * @return next character
     */
    char peekNextChar() const;

    /**
     * @brief Skip until the end of the line
     */
    void skipUntilEndOfLine();

    /**
     * @brief Reads from the current position until the end of the line/string '\n' or '\0'
     *
     * @return std::string of the read contents
     */
    std::string readUntilEndOfLine();

    /**
     * @brief Skips any empty character '\t', ' ', etc.
     */
    void skipEmpty();

    /**
     * @brief Check if the string has been completely walked
     *
     * @return true if walked, false otherwise
     */
    bool isFinished() const;

private:
    /**
     * @brief Check if the passed character is empty
     *
     * @param c character to check
     * @return true if empty, false otherwise
     */
    bool isCharEmpty(const char c) const;

private:
    std::string mString; // String that is being walked
    size_t mIndex = 0;   // Current index of string reading
};
} // namespace mirror::lexer
