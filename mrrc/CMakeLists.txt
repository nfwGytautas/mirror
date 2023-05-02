# Add source to this project's executable.
set(SOURCE_FILES
    src/main.cpp
    src/compiler/Compiler.cpp
    # Lexer
    src/lexer/Lexer.cpp
    src/lexer/StringWalker.cpp
    src/lexer/TokenStream.cpp
    # Tokenizers
    src/lexer/tokenizers/Tokenizer.cpp
    src/lexer/tokenizers/Tokenizers.cpp
    src/lexer/tokenizers/CommentTokenizer.cpp
    # Utility
    src/utility/HelpMessage.hpp
    src/utility/cli/CLIParser.hpp
    src/utility/cli/CLIParser.cpp
)

set(INCLUDE_FILES
    include/mirror/compiler/Compiler.hpp
    # Lexer
    include/mirror/lexer/Lexer.hpp
    include/mirror/lexer/Token.hpp
    include/mirror/lexer/StringWalker.hpp
    include/mirror/lexer/TokenStream.hpp
    # Tokenizers
    include/mirror/lexer/tokenizers/Tokenizer.hpp
    include/mirror/lexer/tokenizers/Tokenizers.hpp
    include/mirror/lexer/tokenizers/CommentTokenizer.hpp
    # Utility
    include/mirror/Errors.hpp
)

set(VERSION
    \"0.0.0\"
)

if(MRRC_BUILD_TYPE STREQUAL "Lib")
    message(STATUS "Generating mirror compiler as library")
    add_library(mrrc ${SOURCE_FILES} ${INCLUDE_FILES})
    target_compile_definitions(mrrc PRIVATE MRRC_LIB_MODE)
else()
    message(STATUS "Generating mirror compiler as a executable")
    add_executable(mrrc ${SOURCE_FILES} ${INCLUDE_FILES})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(mrrc PRIVATE _DEBUG)
endif()

target_compile_definitions(mrrc PRIVATE MRRC_VERSION=${VERSION})

target_include_directories(mrrc PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")
target_include_directories(mrrc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/")

# TODO: Add tests and install targets
